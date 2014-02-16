/*
 * GraphicsItemEvent.h
 */

#ifndef TGL_CORE_GRAPHICSITEMEVENT_H_
#define TGL_CORE_GRAPHICSITEMEVENT_H_

#include "InputEvent.h"

namespace tgl {

class GraphicsItemEvent {
public:
	GraphicsItemEvent();
	virtual ~GraphicsItemEvent();
};

class GraphicsItemHoverEvent : public GraphicsItemEvent
{
public:
	GraphicsItemHoverEvent() {}
	virtual ~GraphicsItemHoverEvent() {}
};

class GraphicsItemMouseEvent : public GraphicsItemEvent, public MouseEvent
{
public:
	GraphicsItemMouseEvent() {}
	virtual ~GraphicsItemMouseEvent() {}
};

class GraphicsItemSelectEvent : public GraphicsItemEvent
{
public:
	GraphicsItemSelectEvent() {}
	virtual ~GraphicsItemSelectEvent() {}
};

} /* namespace tgl */

#endif /* GRAPHICSITEMEVENT_H_ */
