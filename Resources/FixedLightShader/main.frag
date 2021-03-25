#version 450

in vec3 pos;
in vec3 normal;
in vec2 texCoord;


out vec4 FragColor;
uniform sampler2D Diffuse;
uniform vec3 CamPos;
uniform vec3 LightPos;
uniform vec3 LightComps;
uniform vec3 LightColor;
uniform vec3 LightAttenuation;
uniform bool EnableTex;



vec3 calcPointLight(vec3 lightsToCalc[6])
{
    vec3 norm = normalize(normal);
    float shininess = 32;
    vec3 ambiant    = lightsToCalc[1].x * lightsToCalc[2];
    vec3 lightDir   = normalize(lightsToCalc[0] - pos);
    vec3 viewDir    = normalize(CamPos - pos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float gamma = 2.2;
    float distance = length(lightsToCalc[0] - pos);
    float attenuation = 1.0 / (lightsToCalc[3].x + (lightsToCalc[3].y * distance) + (lightsToCalc[3].z * (distance * distance)));
    float attenuation2 = 1.0 / (2.2 * (distance * distance));
    float spec = pow(max(dot(norm, halfwayDir), 0.0), shininess);
    vec3 specular = lightsToCalc[1].z * spec * lightsToCalc[2];
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightsToCalc[1].y * lightsToCalc[2];
    ambiant *= attenuation * attenuation2;
    diffuse *= attenuation;
    specular *= attenuation * attenuation2;
    return (ambiant + diffuse + specular);

}

vec3 calcLight(vec3 lightsToCalc[6])
{
    vec3 lightResult = vec3(0,0,0);
    if (lightsToCalc[5].y == 0)
    lightResult += calcPointLight(lightsToCalc);
    //if (lightsToCalc[5].y == 1)
    //lightResult += calcDirectionalLight(lightsToCalc);
    //if (lightsToCalc[5].y == 2)
    //lightResult += calcSpotLight(lightsToCalc);
    return lightResult;
}


void main()
{
    vec3 tex = vec3(1,1,1);
    if(EnableTex)
        tex = texture(Diffuse, texCoord).rgb;//vec3(1,1,1);
    vec3 lightOutPut = vec3(0,0,0);



    //[0] = pos
    //[1] = component ambiant, Diffuse ,Specular
    //[2] = color
    //[3] = attenuation
    //[4] = lighting dir (Spot only)
    //[5].x = angle
    //[5].y = type of light

    /*for (int i = 0; i < numLight*6 ; i += 6)
    {
        vec3 currentLight[6];
        currentLight[0] = light[i];
        currentLight[1] = light[i+1];
        currentLight[2] = light[i+2];
        currentLight[3] = light[i+3];
        currentLight[4] = light[i+4];
        currentLight[5] = light[i+5];

        lightOutPut += calcLight(currentLight);
    }*/

    vec3 currentLight[6] =
    {
    LightPos,
    LightComps,
    LightColor,
    LightAttenuation,
    {0,0,0},
    {0,0,0},
    };
    lightOutPut += calcLight(currentLight);




    float gamma = 2.2;

    vec3 result = tex * lightOutPut;
    FragColor = vec4(result,1);
    FragColor.rgb = pow(FragColor.rgb, vec3(1.0/gamma));
}