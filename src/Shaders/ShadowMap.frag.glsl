#version 430

uniform mat4 MVP;

layout(location = 0) out float FragmentDepth;

void main() {
	FragmentDepth = gl_FragCoord.z;
}