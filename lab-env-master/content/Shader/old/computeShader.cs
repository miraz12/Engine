#version 430 compatibility
#extension GL_ARB_compute_shader : enable
#extension GL_ARB_shader_storage_buffer_object : enable

struct pos
{
	vec4 pxyzw;	// positions
};

struct vel
{
	vec4 vxyzw;	// velocities
};

struct color
{
	vec4 crgba;	// colors
};

layout( std430, binding=4 ) buffer Pos 
{
	pos  Positions[  ];		// array of structures
};

layout( std430, binding=5 ) buffer Vel 
{
	vel  Velocities[  ];		// array of structures
};

layout( std430, binding=6 ) buffer Col 
{
	color  Colors[  ];		// array of structures
};

layout( local_size_x = 128,  local_size_y = 1, local_size_z = 1 )   in;
uniform mat4 MVP;

void main( )
{
	uint  gid = gl_GlobalInvocationID.x;	// the .y and .z are both 1 in this case

    vec3 p  = Positions[  gid  ].pxyzw.xyz;
    vec3 v  = Velocities[ gid  ].vxyzw.xyz;

	vec3 pp = p + v*0.1f;// + .5*DT*DT*G;
	vec3 vp = v;// + G*DT;
	
	if(length(pp) > 1.f)
	{
		pp = vec3(0,0,0);
	}


    Positions[  gid  ].pxyzw     = vec4( pp, 1. );
    Velocities[ gid  ].vxyzw.xyz = vp;
}
