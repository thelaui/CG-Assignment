# version 330

layout(location=0) in vec3 in_Position;
layout(location=1) in vec3 in_Normal;
layout(location=2) in vec2 in_TexCoord;
layout(location=3) in float in_PointSize;
layout(location=4) in vec4 in_PointColor;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 normalMatrix;
uniform bool isParticle;

uniform sampler2D bumpmap;

out vec3 normal;
out vec3 sunDirection;
out vec3 cameraDirection;
out vec2 texCoord;
out vec4 pointColor;

void main(void) {

    if (isParticle) {
        pointColor      = in_PointColor;

        gl_Position     = (projectionMatrix * viewMatrix) * vec4(in_Position,1.0);
        gl_PointSize    = 10.0*in_PointSize/length((vec4(in_Position,1.0) - inverse(viewMatrix) * vec4(0.0, 0.0, 0.0, 1.0)).xyz);
    }
    else {
        texCoord        = in_TexCoord;
        normal          = normalize((normalMatrix * vec4(in_Normal, 0.0)).xyz);
        sunDirection    = normalize((modelMatrix * vec4(in_Position,1.0)).xyz);
        cameraDirection = normalize((modelMatrix * vec4(in_Position,1.0) - inverse(viewMatrix) * vec4(0.0, 0.0, 0.0, 1.0)).xyz);

        gl_Position     = (projectionMatrix * viewMatrix * modelMatrix) * vec4(in_Position,1.0);
    }
}
