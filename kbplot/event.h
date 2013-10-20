#ifndef EVENT_H
	#define EVENT_H 
class IMouseEventListener {
public:
	virtual ~IMouseEventListener() {};

	virtual void mouseMoveEvent(int,int)=0;
	virtual void mousePressEvent(int,int)=0;
	virtual void mouseReleaseEvent(int,int)=0;
private:
};

#endif
