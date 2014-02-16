/*
 * GraphicsView.cpp
 */

#include "GraphicsDriver.h"
#include "GraphicsView.h"

//#include <iostream>
//using namespace std;

namespace tgl {

GraphicsView::GraphicsView(std::unique_ptr<GraphicsDriver> driver)
	: driver_(std::move(driver))
{
	initialized_ = false;

	renderer3D_ = std::move(std::unique_ptr<Renderer3D>(new Renderer3D(this)));
	renderer2D_ = std::move(std::unique_ptr<Renderer2D>(new Renderer2D(this)));
	textRenderer_ = std::move(std::unique_ptr<TextRenderer>(new TextRenderer(this)));

	mouseEvent_ = std::move(std::unique_ptr<MouseEvent>(new MouseEvent));
	wheelEvent_ = std::move(std::unique_ptr<WheelEvent>(new WheelEvent));
	keyEvent_ = std::move(std::unique_ptr<KeyEvent>(new KeyEvent));

	graphicsItemMouseEvent_ = std::move(std::unique_ptr<GraphicsItemMouseEvent>(new GraphicsItemMouseEvent));
	graphicsItemHoverEvent_ = std::move(std::unique_ptr<GraphicsItemHoverEvent>(new GraphicsItemHoverEvent));
	graphicsItemSelectEvent_ = std::move(std::unique_ptr<GraphicsItemSelectEvent>(new GraphicsItemSelectEvent));

	camera_ = CameraPtr(new StandardCamera);

	frameRate_ = 30;

	perspectiveFovy_ = 45;
	perspectiveZnear_ = 0.01;
	perspectiveZfar_ = 100;

	float perspectiveFovy_;		// 縦の視野角を”度”単位
	float perspectiveZnear_;		// 一番近いZ位置
	float perspectiveZfar_;		// 一番遠いZ位置

	backgroundColor_ = {{0.8, 0.8, 0.8, 1.0}};
}

GraphicsView::~GraphicsView()
{
//	driver_->terminate();
}

void GraphicsView::initialize()
{
	driver_->initialize(this);
}

void GraphicsView::execute()
{
	driver_->execute();
}

void GraphicsView::terminate()
{
	driver_->terminate();
}

void GraphicsView::setWindowTitle(const std::string& title)
{
	driver_->setWindowTitle(title);
}

const std::string& GraphicsView::windowTitle() const
{
	return driver_->windowTitle();
}


void GraphicsView::setWindowSize(int width, int height)
{
	driver_->setWindowSize(width, height);
}

int GraphicsView::width() const
{
	return driver_->width();
}

int GraphicsView::height() const
{
	return driver_->height();
}

void GraphicsView::setFrameRate(int fps)
{
	driver_->setFrameRate(fps);
}

int GraphicsView::frameRate() const
{
	return driver_->frameRate();
}

LightPtr GraphicsView::light(size_t index) const
{
	return index < lightList_.size() ? lightList_[index] : nullptr;
}

void GraphicsView::initializeEvent()
{
	glShadeModel(GL_FLAT);
	glEnable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);

	///----------------
    glClearColor(backgroundColor_[0], backgroundColor_[1], backgroundColor_[2], backgroundColor_[3]);	// ウインドウの背景色の指定

    glShadeModel(GL_SMOOTH);

    glEnable(GL_LINE_SMOOTH);	// アンチエイリアス
    glEnable(GL_POINT_SMOOTH);
//    glEnable(GL_POLYGON_SMOOTH);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    addLight(LightPtr(new Light(0)));
    addLight(LightPtr(new Light(1)));
    light(0)->setPosition({100,100,200});
    light(1)->setPosition({100,-100,200});

    camera_->initializeConfiguration();

    initialized_ = true;
}

void GraphicsView::resizeEvent(int width, int height)
{
	double aspect = (double)width / (double)height;

	glViewport(0, 0, width, height);			// ビューポートの再設定
	glMatrixMode(GL_PROJECTION);				// 投影変換スタックの操作
	glLoadIdentity();								// 投影変関スタックの初期化
	gluPerspective(perspectiveFovy_, aspect, perspectiveZnear_, perspectiveZfar_);	// ビューボリュームの再定義
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	camera_->updateProject();
}

void GraphicsView::renderEvent()
{
	executePrevProcess();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);	// Zバッファ有効

	glLoadIdentity();		// 投影変関スタックの初期化
	// -----------------------------
	glPushMatrix();
	glLoadIdentity();		// 投影変関スタックの初期化

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// set background color
	glClearColor(backgroundColor_[0], backgroundColor_[1], backgroundColor_[2], backgroundColor_[3]);

	// update camera
	camera_->update();
	camera_->updateProject();

	// update light
	for (auto light : lightList_) {
		light->update();
	}

	// get viewport
	glGetIntegerv(GL_VIEWPORT, viewport_.data());

	traverseGraphicsItems(traversedItems_);

	// render scene
	{
		double color[4];
		glGetDoublev(GL_CURRENT_COLOR, color);	// store current color
		renderScene(renderer3D_.get());
		renderSceneOfGrahicsItems();
		glColor4dv(color);						// restore color
	}

	// render overlay scene
	{
		double color[4];
		glGetDoublev(GL_CURRENT_COLOR, color);	// store current color
		glClear(GL_DEPTH_BUFFER_BIT);
		renderOverlayScene(renderer3D_.get());
		renderOverlaySceneOfGrahicsItems();
		glColor4dv(color);						// restore color
	}

	// render 2Dscene
	{
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0, width(), height(), 0);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		// render text
		{
			double color[4];
			glGetDoublev(GL_CURRENT_COLOR, color);	// store current color
			glClear(GL_DEPTH_BUFFER_BIT);
			glDisable(GL_CULL_FACE);
			glDisable(GL_LIGHTING);
			glDisable(GL_DEPTH_TEST);

			render2DScene(renderer2D_.get());
			render2DSceneOfGrahicsItems();

			glColor4dv(color);						// restore color
		}

		// render text
		{
			double color[4];
			glGetDoublev(GL_CURRENT_COLOR, color);	// store current color

			glClear(GL_DEPTH_BUFFER_BIT);
			glDisable(GL_DEPTH_TEST);
			renderTextScene(textRenderer_.get());
			renderTextSceneOfGrahicsItems();

			glColor4dv(color);						// restore color
		}

		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	}

	glPopMatrix();
	// ------------------------------
    glDisable(GL_DEPTH_TEST);		// Zバッファ無効
    glFlush();

    executePostProcess();
}

void GraphicsView::setCamera(CameraPtr camera)
{
	if (!camera) return;

	camera_ = camera;
	camera_->initializeConfiguration();
}

void GraphicsView::addLight(LightPtr light)
{
	if (!light) return;

	lightList_.push_back(light);
	light->initializeConfiguration();
}

void GraphicsView::removeLight(LightPtr light)
{
	auto itr = std::find(lightList_.begin(), lightList_.end(), light);
	if (itr != lightList_.end()) {
		lightList_.erase(itr);
	}
}

// GraphicsItem
void GraphicsView::addGraphicsItem(GraphicsItemPtr item)
{
	if (!item) return;

	graphicsItems_.push_back(item);

	GraphicsItem::traverse(item, [&](GraphicsItemPtr ptr){
		ptr->graphicsView_ = this;
	});
}

void GraphicsView::removeGraphicsItem(GraphicsItemPtr item)
{
	if (!item) return;

	auto itr = std::find(graphicsItems_.begin(), graphicsItems_.end(), item);
	if (itr != graphicsItems_.end()) {
		graphicsItems_.erase(itr);
		(*itr)->graphicsView_ = nullptr;
	}
}

void GraphicsView::setBackgroundColor(double r, double g, double b, double a)
{
	backgroundColor_ = {{r, g, b, a}};
}

// ----- GraphicsItem -----
void GraphicsView::renderSceneOfGrahicsItems()
{
	for (auto item : traversedItems_) {
		if (item->isVisible()) {
			item->renderScene(this->renderer3D_.get());
		}
	}
}

void GraphicsView::renderOverlaySceneOfGrahicsItems()
{
	for (auto item : traversedItems_) {
		if (item->isVisible()) {
			item->renderOverlayScene(this->renderer3D_.get());
		}
	}
}

void GraphicsView::render2DSceneOfGrahicsItems()
{
	for (auto item : traversedItems_) {
		if (item->isVisible()) {
			item->render2DScene(renderer2D_.get());
		}
	}
}

void GraphicsView::renderTextSceneOfGrahicsItems()
{
	for (auto item : traversedItems_) {
		if (item->isVisible()) {
			item->renderTextScene(textRenderer_.get());
		}
	}
}

void GraphicsView::mousePressEvent(MouseEvent* e)
{
	if (e->button() == MouseEvent::MouseBotton::RightButton ||
			e->button() == MouseEvent::MouseBotton::MiddleButton) {
		camera_->mousePressEvent(e);
	}

	graphicsItemMousePressEvent(e);

	// post event
	mousePressPostEvent(e);
}

void GraphicsView::mouseMoveEvent(MouseEvent* e)
{
	if (e->button() == MouseEvent::MouseBotton::RightButton ||
			e->button() == MouseEvent::MouseBotton::MiddleButton) {
		camera_->mouseMoveEvent(e);
	}

    graphicsItemMouseMoveEvent(e);

    // post event
    mouseMovePostEvent(e);
}

void GraphicsView::mouseReleaseEvent(MouseEvent* e)
{
	if (e->button() == MouseEvent::MouseBotton::RightButton ||
			e->button() == MouseEvent::MouseBotton::MiddleButton) {
		camera_->mouseReleaseEvent(e);
	}

	graphicsItemMouseReleaseEvent(e);

	// post event
	mouseReleasePostEvent(e);
}

void GraphicsView::wheelEvent(WheelEvent* e)
{
    camera_->wheelEvent(e);
}

void GraphicsView::keyPressEvent(KeyEvent* e)
{

}

void GraphicsView::graphicsItemMousePressEvent(MouseEvent* e)
{
	if (e->button() == MouseEvent::MouseBotton::LeftButton) {

		GraphicsItemList pickingItems = pickingUpGrahicsItems(e->x(), e->y());

		if (pickingItems.empty()) {

			// picked event
			if (pickedGraphicsItem_) {
				pickedGraphicsItem_->selectLeaveEvent(graphicsItemSelectEvent_.get());
				pickedGraphicsItem_ = nullptr;
			}

		} else {
			GraphicsItemPtr nearItem = pickingItems.front();

			// picked event
			if (pickedGraphicsItem_ != nearItem) {
				if (pickedGraphicsItem_) {
					pickedGraphicsItem_->selectLeaveEvent(graphicsItemSelectEvent_.get());
				}
				pickedGraphicsItem_ = nearItem;
				pickedGraphicsItem_->selectEnterEvent(graphicsItemSelectEvent_.get());
			} else {

			}

			graphicsItemMouseEvent_->setPressEvent(e->x(), e->y(), e->button());

			// mouse event
			mouseGraphicsItem_ = nearItem;
			mouseGraphicsItem_->mousePressEvent(graphicsItemMouseEvent_.get());

		}
	}
}

void GraphicsView::graphicsItemMouseMoveEvent(MouseEvent* e)
{

	graphicsItemMouseEvent_->setMoveEvent(e->x(), e->y(), e->button());

	if (e->button() == MouseEvent::MouseBotton::LeftButton) {

		// mouse event
		if (mouseGraphicsItem_) {
			mouseGraphicsItem_->mouseMoveEvent(graphicsItemMouseEvent_.get());
		}
	}

	GraphicsItemList pickingItems = pickingUpGrahicsItems(e->x(), e->y());

	if (pickingItems.empty()) {

		// mouse hover event
		if (hoveredGraphicsItem_) {
			hoveredGraphicsItem_->hoverLeaveEvent(graphicsItemHoverEvent_.get());
			hoveredGraphicsItem_ = nullptr;
		}

	} else {

		GraphicsItemPtr nearItem = pickingItems.front();

		// mouse hover event
		if (hoveredGraphicsItem_ != nearItem) {
			if (hoveredGraphicsItem_) {
				hoveredGraphicsItem_->hoverLeaveEvent(graphicsItemHoverEvent_.get());
			}
			hoveredGraphicsItem_ = nearItem;
			hoveredGraphicsItem_->hoverEnterEvent(graphicsItemHoverEvent_.get());
		} else {
			hoveredGraphicsItem_->hoverMoveEvent(graphicsItemHoverEvent_.get());
		}
	}
}

void GraphicsView::graphicsItemMouseReleaseEvent(MouseEvent* e)
{
	graphicsItemMouseEvent_->setReleaseEvent(e->x(), e->y(), e->button());

	if (e->button() == MouseEvent::MouseBotton::LeftButton) {
		if (pickedGraphicsItem_) {

		}
	}

	// mouse event
	if (mouseGraphicsItem_) {
		mouseGraphicsItem_->mouseReleaseEvent(graphicsItemMouseEvent_.get());
		mouseGraphicsItem_ = nullptr;
	}
}

GraphicsItemList GraphicsView::pickingUpGrahicsItems(int x, int y)
{
	GraphicsItemList items;

	// 2D scene
	items = pickingUp2DSceneGrahicsItems(x, y);
	if (items.size() > 0) return std::move(items);

	// overlay 3D scene
	items = pickingUpOverlaySceneGrahicsItems(x, y);
	if (items.size() > 0) return std::move(items);

	// 3D scene
	items = pickingUpSceneGrahicsItems(x, y);

	return std::move(items);
}

GraphicsItemList GraphicsView::pickingUpOverlaySceneGrahicsItems(int x, int y)
{
	std::map<int, GraphicsItemPtr> indexToGraphicsItemMap;
	int index = 1;

	const int bufSize = 512;
	GLuint selectBuf[bufSize];
	GLuint hits;
	GLint viewport[4];

	// セレクション開始
	for (int i = 0; i < 4; ++i) { viewport[i] = viewport_[i]; }
	glSelectBuffer(bufSize, selectBuf);

	glRenderMode(GL_SELECT);				// セレクションに移行

	glInitNames();	// nameバッファの初期化
	glPushName(0);

	glMatrixMode(GL_PROJECTION);

	// picking 3D scene
	{
		glPushMatrix();
		glLoadIdentity();
		gluPickMatrix(x, viewport[3]-y, 5.0, 5.0, viewport); // ピッキング行列の乗算
		float aspect = (float)viewport[2] / (float)viewport[3];
		gluPerspective(perspectiveFovy_, aspect, perspectiveZnear_, perspectiveZfar_);	// ビューボリュームの再定義

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		camera_->update();					// カメラ再設定
//		camera_->updateProject();

		for (auto item : traversedItems_) {
			if (item->isVisible()) {
				glLoadName(index);
				item->renderPickingOverlayScene(renderer3D_.get());
				indexToGraphicsItemMap[index] = item;
				++index;
			}
		}

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
	}

	hits = glRenderMode(GL_RENDER);			// ヒットレコード
	glMatrixMode(GL_MODELVIEW);

	// selectHitsGrahicsItem
	return selectHitsGrahicsItems(hits, selectBuf, indexToGraphicsItemMap);
}

GraphicsItemList GraphicsView::pickingUpSceneGrahicsItems(int x, int y)
{
	std::map<int, GraphicsItemPtr> indexToGraphicsItemMap;
	int index = 1;

	const int bufSize = 512;
	GLuint selectBuf[bufSize];
	GLuint hits;
	GLint viewport[4];

	// セレクション開始
//	glGetIntegerv(GL_VIEWPORT, viewport);
	for (int i = 0; i < 4; ++i) { viewport[i] = viewport_[i]; }
	glSelectBuffer(bufSize, selectBuf);

	glRenderMode(GL_SELECT);				// セレクションに移行

	glInitNames();	// nameバッファの初期化
	glPushName(0);

	glMatrixMode(GL_PROJECTION);

	// picking 3D scene
	{
		glPushMatrix();
		glLoadIdentity();
		gluPickMatrix(x, viewport[3]-y, 5.0, 5.0, viewport); // ピッキング行列の乗算
		float aspect = (float)viewport[2] / (float)viewport[3];
		gluPerspective(perspectiveFovy_, aspect, perspectiveZnear_, perspectiveZfar_);	// ビューボリュームの再定義


		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		camera_->update();					// カメラ再設定
//		camera_->updateProject();

		for (auto item : traversedItems_) {
			if (item->isVisible()) {
				glLoadName(index);
				item->renderPickingScene(renderer3D_.get());
				indexToGraphicsItemMap[index] = item;
				++index;
			}
		}

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
	}

	hits = glRenderMode(GL_RENDER);			// ヒットレコード
	glMatrixMode(GL_MODELVIEW);

	// selectHitsGrahicsItem
	return selectHitsGrahicsItems(hits, selectBuf, indexToGraphicsItemMap);
}

GraphicsItemList GraphicsView::pickingUp2DSceneGrahicsItems(int x, int y)
{
	std::map<int, GraphicsItemPtr> indexToGraphicsItemMap;
	int index = 1;

	const int bufSize = 512;
	GLuint selectBuf[bufSize];
	GLuint hits;
	GLint viewport[4];

	// セレクション開始
	for (int i = 0; i < 4; ++i) { viewport[i] = viewport_[i]; }
	glSelectBuffer(bufSize, selectBuf);

	glRenderMode(GL_SELECT);				// セレクションに移行

	glInitNames();	// nameバッファの初期化
	glPushName(0);

	glMatrixMode(GL_PROJECTION);

	// picking 2D
	{
		glPushMatrix();
		glLoadIdentity();
		gluPickMatrix(x, viewport[3]-y, 5.0, 5.0, viewport); // ピッキング行列の乗算
		gluOrtho2D(0, width(), height(), 0);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		for (auto item : traversedItems_) {
			if (item->isVisible()) {
				glLoadName(index);
				item->renderPicking2DScene(renderer2D_.get());
				indexToGraphicsItemMap[index] = item;
				++index;
			}
		}

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
	}

	hits = glRenderMode(GL_RENDER);			// ヒットレコード
	glMatrixMode(GL_MODELVIEW);

	// selectHitsGrahicsItem
	return selectHitsGrahicsItems(hits, selectBuf, indexToGraphicsItemMap);
}

GraphicsItemList GraphicsView::selectHitsGrahicsItems(GLuint hits, GLuint* buf, const std::map<int, GraphicsItemPtr>& indexToGraphicsItemMap)
{
	GraphicsItemList pickedGraphicsItems;
	std::map<GLuint, GraphicsItemPtr> depthGraphicsItemMap;

	// selectHitsGrahicsItem
	GLuint depth_1 = 1;
	GLuint depth_2 = 1;
	GLuint depth_name;
	GLuint* ptr;

	ptr = buf;

	for (unsigned int i = 0; i < hits; ++i) {
		depth_name = *ptr;				// 階層の深さ
		++ptr;
		depth_1 = *ptr;					// depth_min
		++ptr;
		depth_2 = *ptr;					// depth_max
		++ptr;

		std::vector<GLuint> index;
		for (unsigned int j = 0; j < depth_name; ++j) {
			index.push_back(*ptr);
			++ptr;
		}

		GraphicsItemPtr item = indexToGraphicsItemMap.at(index[0]);

		depthGraphicsItemMap[depth_1] = item;
	}

	// depth_min 順にソート
	for (auto itr = depthGraphicsItemMap.begin(); itr != depthGraphicsItemMap.end(); ++itr) {
		GraphicsItemPtr item = itr->second;
		if (pickedGraphicsItems.end() == std::find(pickedGraphicsItems.begin(), pickedGraphicsItems.end(), item)) {
			if (item) {
				pickedGraphicsItems.push_back(item);
			}
		}
	}

	return pickedGraphicsItems;
}

void GraphicsView::traverseGraphicsItems(GraphicsItemList& items)
{
	items.clear();

	for (auto item : graphicsItems_) {
		GraphicsItem::traverse(item, [&](GraphicsItemPtr ptr){
			items.push_back(ptr);
		});
	}
}


} /* namespace tgl */
