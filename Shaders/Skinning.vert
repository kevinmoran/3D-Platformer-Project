#version 140

in vec3 vp;
in vec3 vn;
// in vec2 vt;
in uvec4 boneIDs;
in vec4 boneWeights;

uniform mat4 M, V, P;
uniform mat4 poseMats[100];

//out vec2 texCoords;
out vec3 normal;

void main () {
	// texCoords = vt;
	normal = vn;

	mat4 boneTransform = 
	(poseMats[boneIDs[0]] * boneWeights[0]) +
	(poseMats[boneIDs[1]] * boneWeights[1]) +
	(poseMats[boneIDs[2]] * boneWeights[2]) +
	(poseMats[boneIDs[3]] * boneWeights[3]);

	gl_Position = P*V*M * boneTransform * vec4(vp, 1.0);
}
