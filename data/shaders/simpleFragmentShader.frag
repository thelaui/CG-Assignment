#version 330

in vec3 normal;
in vec3 sunDirection;
in vec3 cameraDirection;
in vec2 texCoord;
in vec4 pointColor;


uniform vec3 atmoColor;
uniform bool isBillboard;
uniform bool isParticle;

uniform sampler2D diffuse;
uniform sampler2D bumpmap;
uniform sampler2D specular;
uniform sampler2D emit;

out vec4 out_Color;

void main(void) {

    if (isParticle) {
        out_Color = texture2D(diffuse, gl_PointCoord)*pointColor;
    }
    else if (isBillboard) {
        vec3 shaded = (texture2D(diffuse, texCoord).rgb)*atmoColor;
        out_Color = vec4(shaded, 1.0);
    }
    else {
        vec3 bump = normalize(texture2D(bumpmap, texCoord).rgb);
        vec3 bumpedNormal = normalize(( normal + bump - 0.5 ));

        float diffuseShade = max(dot(-sunDirection, bumpedNormal), 0.0);
        float specularShade = pow(max(dot(cameraDirection, reflect(-sunDirection, bumpedNormal)), 0.0), 7)*texture2D(specular, texCoord).r;
        float atmoShade = max(dot(-cameraDirection, normal), 0.0);

        if (atmoShade > 0.0)
            atmoShade = pow(1.0 - atmoShade, 3);

        vec3 atmoColorShaded = atmoColor * atmoShade;

        vec3 shaded = (texture2D(diffuse, texCoord).rgb)*diffuseShade + texture2D(emit, texCoord).rgb*(1-diffuseShade)  + atmoColorShaded + specularShade;

        out_Color = vec4(shaded, 1.0);
    }

}
