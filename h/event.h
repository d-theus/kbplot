//---------------------------------------------------------------------------

#ifndef EVENT_H
#define EVENT_H

class IMouseEventListener {
public:
	virtual void mouseMoveEvent(int,int)=0;
	virtual void mousePressedEvent(int,int)=0;
	virtual void mouseReleaseEvent(int,int)=0;
	virtual void mouseScrollEvent(int)=0;
};

#endif//EVENT_H
