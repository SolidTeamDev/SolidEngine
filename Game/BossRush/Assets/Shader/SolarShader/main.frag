#version 450

in vec2 vUV;
in vec3 vWorldPos;
in vec3 vNormal;
in float time;

uniform vec2 resolution;

// shadertoy emulation
#define iTime time
#define iResolution vec3(resolution,1.)

// --------[ Original ShaderToy begins here ]---------- //
#define R(p,a,r)mix(a*dot(p,a),p,cos(r))+sin(r)*cross(p,a)
#define H(h)cos(h*6.3+vec3(0,23,21))*.5+.5
void mainImage(out vec4 O, vec2 C)
{
    O=vec4(0);
    vec3 r=iResolution,p;
    float g=0.,e=0.,s=0.;
    for(float i=0.;i<99.;++i)
    {
        p=g*vec3((C-.5*r.xy)/r.y,1);
        p-=vec3(.02,.01,.08);
        p=R(p,normalize(vec3(1,3,3)),iTime*.000001);
        p+=p;
        s=2.;
        for(int j=0;j<7;++j)
            s*=e=1.8/min(dot(p,p),1.2),
            p=abs(p)*e-3.;
        g+=e=length(p.yz)/s;
        (e<.005)?O.xyz+=mix(r/r,H(g*.5),.6)*.05*exp(-g*10.):p;
    }
}
// --------[ Original ShaderToy ends here ]---------- //

void main(void)
{
    mainImage(gl_FragColor, gl_FragCoord.xy);
    gl_FragColor.a = 1.;
}
































