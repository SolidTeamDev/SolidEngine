#version 450

out vec4 fragColor;

in vec2 vUV;
in vec3 vWorldPos;
in vec3 vNormal;
in float time;


uniform vec3 _camPos;

// Material
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;

uniform vec3 albedoValue;
uniform float metallicValue;
uniform float roughnessValue;

uniform bool isUsingNormalMap;
uniform bool isUsingMetallicMap;
uniform bool isUsingRoughnessMap;
uniform bool isUsingAoMap; 
uniform float ttt;

uniform vec2 resolution;


#define TAU 6.28318530718
#define MAX_ITER 4
//#define SHOW_TILING
// Lights


struct Light
{
    int  type;
    vec3 pos;
    vec3 color;
    float intensity;
    vec3  dir;
};

uniform Light _lights[100];
uniform int _nbLights;

const float PI =  3.14159265359;


vec3 WaterColor(void)
{
    
	float t = time * 0.00003;
    // uv should be the 0-1 uv of texture...
	vec2 uv = gl_FragCoord.xy / resolution.xy;


#ifdef SHOW_TILING
	vec2 p = mod(uv*TAU*2.0, TAU)-250.0;
#else
    vec2 p = mod(uv*TAU, TAU)-250.0;
#endif
	vec2 i = vec2(p);
	float c = 1.0;
	float inten = .005;

	for (int n = 1; n < MAX_ITER; n++)
	{
		float t = t * (1.0 - (3.5 / float(n+1)));
		i = p + vec2(cos(t - i.x) + sin(t + i.y), sin(t - i.y) + cos(t + i.x));
		c += 1.0/length(vec2(p.x / (sin(i.x+t)/inten),p.y / (cos(i.y+t)/inten)));
	}
	c /= float(MAX_ITER);
	c = 1.17-pow(c, 1.4);
	vec3 colour = vec3(pow(abs(c), 8.0));
    	colour = clamp(colour + albedoValue, 0.0, 1.0);

#ifdef SHOW_TILING
	// Flash tile borders...
	vec2 pixel = 2.0 / resolution.xy;
	uv *= 2.0;

	float f = floor(mod(time*.5, 2.0)); 	// Flash value.
	vec2 first = step(pixel, uv) * f;		   	// Rule out first screen pixels and flash.
	uv  = step(fract(uv), pixel);				// Add one line of pixels per tile.
	colour = mix(colour, vec3(1.0, 1.0, 0.0), (uv.x + uv.y) * first.x * first.y); // Yellow line

#endif

	return vec3(colour);
}



// ----------------------------------------------------------------------------
// Easy trick to get tangent-normals to world-space to keep PBR code simplified.
// Don't worry if you don't get what's going on; you generally want to do normal
// mapping the usual way for performance anways; I do plan make a note of this
// technique somewhere later in the normal mapping tutorial.
vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(normalMap, vUV).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(vWorldPos);
    vec3 Q2  = dFdy(vWorldPos);
    vec2 st1 = dFdx(vUV);
    vec2 st2 = dFdy(vUV);

    vec3 N   = normalize(vNormal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}
// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}

vec3 PBRDirLight(Light light, vec3 N, vec3 V, vec3 F0, vec3 albedo, float roughness, float metallic)
{
    // calculate per-light radiance
    vec3 L = normalize(-light.dir);
    vec3 H = normalize(V + L);

    // Cook-Torrance BRDF
    float NDF = DistributionGGX(N, H, roughness);
    float G   = GeometrySmith(N, V, L, roughness);
    vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 nominator    = NDF * G * F;
    float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;// 0.001 to prevent divide by zero.
    vec3 specular = nominator / denominator;

    // kS is equal to Fresnel
    vec3 kS = F;
    // for energy conservation, the diffuse and specular light can't
    // be above 1.0 (unless the surface emits light); to preserve this
    // relationship the diffuse component (kD) should equal 1.0 - kS.
    vec3 kD = vec3(1.0) - kS;
    // multiply kD by the inverse metalness such that only non-metals
    // have diffuse lighting, or a linear blend if partly metal (pure metals
    // have no diffuse light).
    kD *= 1.0 - metallic;

    // scale light by NdotL
    float NdotL = max(dot(N, L), 0.0);

    // add to outgoing radiance Lo
    return (kD * albedo / PI + specular) * (light.color * light.intensity) * NdotL;// note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
}

vec3 PBRPointLight(Light light, vec3 N, vec3 V, vec3 F0, vec3 albedo, float roughness, float metallic)
{
    // calculate per-light radiance
    vec3 L = normalize(light.pos - vWorldPos);
    vec3 H = normalize(V + L);
    float distance = length(light.pos - vWorldPos);
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance = (light.color * light.intensity) * attenuation;

    // Cook-Torrance BRDF
    float NDF = DistributionGGX(N, H, roughness);
    float G   = GeometrySmith(N, V, L, roughness);
    vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 nominator    = NDF * G * F;
    float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;// 0.001 to prevent divide by zero.
    vec3 specular = nominator / denominator;

    // kS is equal to Fresnel
    vec3 kS = F;
    // for energy conservation, the diffuse and specular light can't
    // be above 1.0 (unless the surface emits light); to preserve this
    // relationship the diffuse component (kD) should equal 1.0 - kS.
    vec3 kD = vec3(1.0) - kS;
    // multiply kD by the inverse metalness such that only non-metals
    // have diffuse lighting, or a linear blend if partly metal (pure metals
    // have no diffuse light).
    kD *= 1.0 - metallic;

    // scale light by NdotL
    float NdotL = max(dot(N, L), 0.0);

    // add to outgoing radiance Lo
    return (kD * albedo / PI + specular) * radiance * NdotL;// note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
}

vec3 PBRSpotLight()
{
    /*// calculate per-light radiance
    vec3 L = normalize(light.pos - vWorldPos);
    vec3 H = normalize(V + L);
    float distance = length(light.pos - vWorldPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    vec3 radiance = (light.color * light.intensity) * attenuation;

    // Cook-Torrance BRDF
    float NDF = DistributionGGX(N, H, roughness);
    float G   = GeometrySmith(N, V, L, roughness);
    vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 nominator    = NDF * G * F;
    float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;// 0.001 to prevent divide by zero.
    vec3 specular = nominator / denominator;

    // kS is equal to Fresnel
    vec3 kS = F;
    // for energy conservation, the diffuse and specular light can't
    // be above 1.0 (unless the surface emits light); to preserve this
    // relationship the diffuse component (kD) should equal 1.0 - kS.
    vec3 kD = vec3(1.0) - kS;
    // multiply kD by the inverse metalness such that only non-metals
    // have diffuse lighting, or a linear blend if partly metal (pure metals
    // have no diffuse light).
    kD *= 1.0 - metallic;

    // scale light by NdotL
    float NdotL = max(dot(N, L), 0.0);

    // add to outgoing radiance Lo
    return (kD * albedo / PI + specular) * radiance * NdotL;// note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    */
    return vec3(0);
}

vec4 PBRShading()
{
    // Set components value
    vec3  albedo    = WaterColor();
    float metallic  = isUsingMetallicMap ? texture(metallicMap, vUV).r : metallicValue;
    float roughness = isUsingRoughnessMap ? texture(roughnessMap, vUV).r : roughnessValue;
    float ao        = isUsingAoMap ? texture(aoMap, vUV).r : 1.f;

    vec3 N = isUsingNormalMap ? getNormalFromMap() : normalize(vNormal);
    vec3 V = normalize(_camPos - vWorldPos);

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < _nbLights; ++i)
    {
        Light curLight = _lights[i];
        
        if(curLight.type == 0) //Directional
            Lo += PBRDirLight(curLight,N,V,F0,albedo,roughness,metallic);
        else if(curLight.type == 1) //Point
            Lo += PBRPointLight(curLight,N,V,F0,albedo,roughness,metallic);
        else if(curLight.type == 2) // Spot
            Lo += PBRSpotLight();
    }
    // ambient lighting (note that the next IBL tutorial will replace
    // this ambient lighting with environment lighting).
    vec3 ambient = vec3(0.03) * albedo * ao;

    vec3 color = ambient + Lo;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2));

    return vec4(color, 1.0);
}

void main()
{
    fragColor = PBRShading();
}

