#include "utilities.h"

Txy myglTrObjectScreen(double x,double y){
	GLdouble scrx,scry,scrz;

	GLint vp[4];
	GLdouble mv[16];
	GLdouble pj[16];

	glGetDoublev(GL_MODELVIEW_MATRIX,mv);
	glGetDoublev(GL_PROJECTION_MATRIX,pj);
	glGetIntegerv(GL_VIEWPORT,vp);

	y = (float)vp[3] - y; //everybody: 0 - topleft, gl:bottomleft >_<

	gluProject(x,y,0.0, mv,pj,vp,&scrx,&scry,&scrz);

	return Txy(scrx,scry);
}

Txy myglTrScreenObject(double x, double y){
	GLdouble ox,oy,oz;

	GLint vp[4];
	GLdouble mv[16];
	GLdouble pj[16];

	glGetDoublev(GL_MODELVIEW_MATRIX,mv);
	glGetDoublev(GL_PROJECTION_MATRIX,pj);
	glGetIntegerv(GL_VIEWPORT,vp);

	y = (float)vp[3] - y; //everybody: 0 - topleft, gl:bottomleft >_<

	gluUnProject(x,y,0.0, mv,pj,vp,&ox,&oy,&oz);

	return Txy(ox,oy);
}

Txy myglPlaneWH(){
	GLint vp[4];
	glGetIntegerv(GL_VIEWPORT,vp);
	Txy tl = myglTrScreenObject(0,0), br = myglTrScreenObject(vp[2],vp[3]);
	return Txy(br.x-tl.x, - (br.y - tl.y));
}

Txy myglScreenWH(){
	GLint vp[4];
	glGetIntegerv(GL_VIEWPORT,vp);
	return Txy(vp[2],vp[3]);
}
