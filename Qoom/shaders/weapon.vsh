vs_2_0

dcl_position v0
dcl_texcoord0 v1
dcl_normal v2
dcl_binormal v3
dcl_tangent v4

;c4 - camera pos
;c5 - light source
;c6 - pos
def c10, 0.5f, 0.7f, 0.9f, 1.0f
def c11, 0.0f, 2.0f, 0.0f, 0.0f

m4x4 oPos, v0, c0
m4x4 r1, v0, c6		;	world pos

sub r0, c5, r1	;	vLight.xyz - pos.xyz

m4x4 r2, v2, c6
mov r4, c4

mov oT0, v1	;tex
mov oT1, r2	;normal
mov oT2, r4	;view
mov oT3, r0 ;light_dir