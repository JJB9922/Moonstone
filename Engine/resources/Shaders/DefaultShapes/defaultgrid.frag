#version 330 core

in vec3 FragPos;
out vec4 FragColor;

const vec3 MAJOR_GRID_COLOR = vec3(0.4, 0.4, 0.4);
const vec3 MINOR_GRID_COLOR = vec3(0.3, 0.3, 0.3);
const vec3 BACKGROUND_COLOR = vec3(0.15, 0.15, 0.15);
const vec3 X_AXIS_COLOR = vec3(1.0, 0.0, 0.0);
const vec3 Z_AXIS_COLOR = vec3(0.0, 0.0, 1.0);
const float MAJOR_GRID_SIZE = 8.0;
const float MINOR_GRID_SIZE = 0.6;
const float MAJOR_LINE_WIDTH = 0.075;
const float MINOR_LINE_WIDTH = 0.025;
const float FADE_DISTANCE = 75.0;
const float AXIS_LINE_WIDTH = 0.05;

float getGrid(vec2 position, float size, float lineWidth) {
    vec2 coord = position / size;
    vec2 grid = abs(fract(coord - 0.5) - 0.5) / fwidth(coord);
    float line = min(grid.x, grid.y);
    return 1.0 - min(line, 1.0);
}

float getAxis(float pos, float width) {
    float edge = width / 2.0;
    return 1.0 - smoothstep(0.0, edge, abs(pos));
}

void main() {
    float distanceToCamera = length(FragPos);

    float majorGrid = getGrid(FragPos.xz, MAJOR_GRID_SIZE, MAJOR_LINE_WIDTH);
    float minorGrid = getGrid(FragPos.xz, MINOR_GRID_SIZE, MINOR_LINE_WIDTH);

    float gridStrength = max(majorGrid, minorGrid * 0.5);
    vec3 gridColor = mix(MINOR_GRID_COLOR, MAJOR_GRID_COLOR, float(majorGrid > 0.0));

    float distanceFromOrigin = length(FragPos);
    float fadeFactor = 1.0 - smoothstep(0.0, FADE_DISTANCE, distanceFromOrigin);

    float xAxis = getAxis(FragPos.z, AXIS_LINE_WIDTH);
    float zAxis = getAxis(FragPos.x, AXIS_LINE_WIDTH);

    vec3 finalColor = mix(BACKGROUND_COLOR, gridColor, gridStrength * fadeFactor);
    finalColor = mix(finalColor, X_AXIS_COLOR, xAxis);
    finalColor = mix(finalColor, Z_AXIS_COLOR, zAxis);

    FragColor = vec4(finalColor, 1.0);
}
