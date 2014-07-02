vs_1_1

dcl_position v0
dcl_texcoord0 v1
dcl_normal v2
dcl_binormal v3
dcl_tangent v4

def c10, 0.5f, 0.7f, 0.9f, 1.0f

mov r0, v0
m4x4 oPos, v0, c0
mov oD0, v0
mov oT0, v1