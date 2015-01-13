void main()
{
	gl_FrontColor = gl_Color; //pass the colour along to the next pahse (fragment shader)

	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
}