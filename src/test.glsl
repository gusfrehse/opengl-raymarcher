#version 440 core
layout (location = 0) in vec2 pos;
out vec2 uv;
void main()
{
    gl_Position = vec4(pos, 0.0, 1.0);
    uv = pos;
}
arandomstring
#version 440 core
in vec2 uv;
out vec4 FragColor;

float sphereSDF(vec3 point, vec3 sphere, float radius)
{
    return length(point - sphere) - radius;

}

float worldSDF(vec3 point)
{
    vec3 sphere = vec3(0.0, 0.0, -5.0);
    float radius = 1.0;
    float dist = sphereSDF(point, sphere, radius);
    dist = min(sphereSDF(point, sphere vec3(0.0, 3.0, -5.0), radius), dist);
    dist = min(sphereSDF(point, sphere vec3(0.8, 0.0, -2.0), radius), dist);
    dist = min(sphereSDF(point, sphere vec3(-4.0, 0.0, -5.0), radius), dist);
    return dist;
}

vec3 calculateNormal(vec3 point)
{
    const vec3 small = vec3(0.001, 0.0, 0.0);
    vec3 gradient = vec3(0.0);
    gradient.x = worldSDF(point + small.xyy) - worldSDF(point - small.xyy);
    gradient.y = worldSDF(point + small.yxy) - worldSDF(point - small.yxy);
    gradient.z = worldSDF(point + small.yyx) - worldSDF(point - small.yyx);
    return normalize(gradient);
}

void main()
{
    const vec3 cameraPosition = vec3(0.0, 0.0,  0.0);
    const vec3 cameraFront = vec3(0.0, 0.0, -1.0);
    const vec3 cameraUp = vec3(0.0, 1.0, 0.0);
    const vec3 cameraRight = cross(cameraFront, cameraUp); // 1 0 0
    const vec3 dir = normalize(cameraFront + (uv.x * cameraRight)+(uv.y * cameraUp));

    const vec3 lightPosition = vec3(-2.0, 3.0, 0.0);
    const vec3 lightColor = vec3(1.0, 0.82, 0.17);

    const int maxSteps = 100;
    float stepSize = 0.001;

    vec3 point = cameraPosition;
    vec3 color = vec3(0.1, 0.1, 0.1);
    int i = 0;
    while (i < maxSteps)
    {
	float dist = worldSDF(point);
	if (abs(dist) < 0.0001)
	{
	    vec3 normal = calculateNormal(point);
	    color = clamp(dot(normal, lightPosition - point), 0.0, 1.0) *
		lightColor;
	    break;
	}
	stepSize = dist;
	point += dir * stepSize;
	i++;
    }

    FragColor = vec4(color, 1.0);
}
