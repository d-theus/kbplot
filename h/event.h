//---------------------------------------------------------------------------

#ifndef eventH
#define eventH

class IMouseEventListener {
public:
	virtual void mouseMoveEvent(int,int)=0;
	virtual void mousePressedEvent(int,int)=0;
	virtual void mouseReleaseEvent(int,int)=0;
	virtual void mouseScrollEvent(int)=0;
};

#endif
