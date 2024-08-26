#version 330 core

in vec3 FragPos;
out vec4 FragColor;

void main() {

    if (mod(abs(FragPos.x), 1.0) < 0.01 || mod(abs(FragPos.z), 1.0) < 0.01) {
        FragColor = vec4(0.5, 0.5, 0.5, 0.5);
    } else {
        FragColor = vec4(0.15, 0.15, 0.15, 0.0);
    }
}
