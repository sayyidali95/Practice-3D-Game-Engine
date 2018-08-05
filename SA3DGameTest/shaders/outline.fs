out vec4 color;



in vec2 TexCoords;

// first pass render target diffuse
uniform sampler2D texture_diffuse1;

//second pass get the normals
uniform sampler2D texture_normal1;



void main(void)
{
	float dx = 1.0 / 128.0;
	float dy = 1.0 / 128.0;

	vec3 center = sampleNrm( texture_normal1, vec2(0.0, 0.0) );

	// sampling just these 3 neighboring fragments keeps the outline thin.
  vec3 top = sampleNrm( texture_normal1, vec2(0.0, dy) );
  vec3 topRight = sampleNrm( texture_normal1, vec2(dx, dy) );
  vec3 right = sampleNrm( texture_normal1, vec2(dx, 0.0) );

  // the rest is pretty arbitrary, but seemed to give me the
  // best-looking results for whatever reason.

  vec3 t = center - top;
  vec3 r = center - right;
  vec3 tr = center - topRight;

  t = abs( t );
  r = abs( r );
  tr = abs( tr );

  float n;
  n = max( n, t.x );
  n = max( n, t.y );
  n = max( n, t.z );
  n = max( n, r.x );
  n = max( n, r.y );
  n = max( n, r.z );
  n = max( n, tr.x );
  n = max( n, tr.y );
  n = max( n, tr.z );

  // threshold and scale.
  n = 1.0 - clamp( clamp((n * 2.0) - 0.8, 0.0, 1.0) * 1.5, 0.0, 1.0 );


  color.rgb = texture(texture_diffuse1, TexCoords).rgb * (0.1 + 0.9*n);

}