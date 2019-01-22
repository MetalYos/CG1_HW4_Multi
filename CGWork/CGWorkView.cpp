// CGWorkView.cpp : implementation of the CCGWorkView class
//
#include "ALMath.h"
#include "stdafx.h"
#include "CGWork.h"

#include "CGWorkDoc.h"
#include "CGWorkView.h"

#include <iostream>
using std::cout;
using std::endl;
#include "MaterialDlg.h"
#include "LightDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "PngWrapper.h"
#include "iritSkel.h"

#include <assert.h>
#include <algorithm>
#include "Scene.h"
#include "CFogFialog.h"
#include <sstream> // TEMP

#include "PngWrapper.h"
// For Status Bar access
#include "MainFrm.h"

// Use this macro to display text messages in the status bar.
#define STATUS_BAR_TEXT(str) (((CMainFrame*)GetParentFrame())->getStatusBar().SetWindowText(str))

#define D_PERSP // activates Perspective with d param


/////////////////////////////////////////////////////////////////////////////
// CCGWorkView

IMPLEMENT_DYNCREATE(CCGWorkView, CView)

BEGIN_MESSAGE_MAP(CCGWorkView, CView)
	//{{AFX_MSG_MAP(CCGWorkView)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_COMMAND(ID_FILE_LOAD, OnFileLoad)
	ON_COMMAND(ID_VIEW_ORTHOGRAPHIC, OnViewOrthographic)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ORTHOGRAPHIC, OnUpdateViewOrthographic)
	ON_COMMAND(ID_VIEW_PERSPECTIVE, OnViewPerspective)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PERSPECTIVE, OnUpdateViewPerspective)
	ON_COMMAND(ID_ACTION_ROTATE, OnActionRotate)
	ON_UPDATE_COMMAND_UI(ID_ACTION_ROTATE, OnUpdateActionRotate)
	ON_COMMAND(ID_ACTION_SCALE, OnActionScale)
	ON_UPDATE_COMMAND_UI(ID_ACTION_SCALE, OnUpdateActionScale)
	ON_COMMAND(ID_ACTION_TRANSLATE, OnActionTranslate)
	ON_UPDATE_COMMAND_UI(ID_ACTION_TRANSLATE, OnUpdateActionTranslate)
	ON_COMMAND(ID_AXIS_X, OnAxisX)
	ON_UPDATE_COMMAND_UI(ID_AXIS_X, OnUpdateAxisX)
	ON_COMMAND(ID_AXIS_Y, OnAxisY)
	ON_UPDATE_COMMAND_UI(ID_AXIS_Y, OnUpdateAxisY)
	ON_COMMAND(ID_AXIS_Z, OnAxisZ)
	ON_UPDATE_COMMAND_UI(ID_AXIS_Z, OnUpdateAxisZ)
	ON_COMMAND(ID_LIGHT_SHADING_FLAT, OnLightShadingFlat)
	ON_UPDATE_COMMAND_UI(ID_LIGHT_SHADING_FLAT, OnUpdateLightShadingFlat)
	ON_COMMAND(ID_LIGHT_SHADING_GOURAUD, OnLightShadingGouraud)
	ON_UPDATE_COMMAND_UI(ID_LIGHT_SHADING_GOURAUD, OnUpdateLightShadingGouraud)
	ON_COMMAND(ID_LIGHT_CONSTANTS, OnLightConstants)
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_BUTTON_BBOX, &CCGWorkView::OnButtonBbox)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_BBOX, &CCGWorkView::OnUpdateButtonBbox)
	ON_COMMAND(ID_BUTTON_VERT_NORM, &CCGWorkView::OnButtonVertNorm)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_VERT_NORM, &CCGWorkView::OnUpdateButtonVertNorm)
	ON_COMMAND(ID_BUTTON_POLY_NORM, &CCGWorkView::OnButtonPolyNorm)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_POLY_NORM, &CCGWorkView::OnUpdateButtonPolyNorm)
	ON_COMMAND(ID_NORMAL_CALCULATED, &CCGWorkView::OnNormalCalculated)
	ON_COMMAND(ID_BUTTON_COLORS, &CCGWorkView::OnButtonColors)
	ON_COMMAND(ID_OPTIONS_MOUSESENSITIVITY, &CCGWorkView::OnOptionsMousesensitivity)
	ON_COMMAND(ID_OPTIONS_TESSELLATIONTOLERANCE, &CCGWorkView::OnOptionsTessellationtolerance)
	ON_COMMAND(ID_NORMAL_FROMFILE, &CCGWorkView::OnNormalFromfile)
	ON_COMMAND(ID_BUTTON_VIEW, &CCGWorkView::OnButtonView)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_VIEW, &CCGWorkView::OnUpdateButtonView)
	ON_COMMAND(ID_BUTTON_OBJECT, &CCGWorkView::OnButtonObject)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_OBJECT, &CCGWorkView::OnUpdateButtonObject)
	ON_COMMAND(ID_OPTIONS_PERSPECTIVECONTROL, &CCGWorkView::OnOptionsPerspectivecontrol)
	ON_COMMAND(ID_ACTION_SELECT, &CCGWorkView::OnActionSelect)
	ON_UPDATE_COMMAND_UI(ID_ACTION_SELECT, &CCGWorkView::OnUpdateActionSelect)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_RENDERING_WIREFRAME, &CCGWorkView::OnRenderingWireframe)
	ON_UPDATE_COMMAND_UI(ID_RENDERING_WIREFRAME, &CCGWorkView::OnUpdateRenderingWireframe)
	ON_COMMAND(ID_RENDERING_SOLIDONSCREEN, &CCGWorkView::OnRenderingSolidonscreen)
	ON_UPDATE_COMMAND_UI(ID_RENDERING_SOLIDONSCREEN, &CCGWorkView::OnUpdateRenderingSolidonscreen)
	ON_COMMAND(ID_RENDERING_SOLIDTOFILE, &CCGWorkView::OnRenderingSolidtofile)
	ON_COMMAND(ID_BACKGROUND_STRETCH, &CCGWorkView::OnBackgroundStretch)
	ON_UPDATE_COMMAND_UI(ID_BACKGROUND_STRETCH, &CCGWorkView::OnUpdateBackgroundStretch)
	ON_COMMAND(ID_BACKGROUND_REPEAT, &CCGWorkView::OnBackgroundRepeat)
	ON_UPDATE_COMMAND_UI(ID_BACKGROUND_REPEAT, &CCGWorkView::OnUpdateBackgroundRepeat)
	ON_COMMAND(ID_BACKGROUND_OPEN, &CCGWorkView::OnBackgroundOpen)
	ON_COMMAND(ID_LIGHT_SETMATERIAL, &CCGWorkView::OnLightSetmaterial)
	ON_COMMAND(ID_LIGHT_SHADING_PHONG, &CCGWorkView::OnLightShadingPhong)
	ON_UPDATE_COMMAND_UI(ID_LIGHT_SHADING_PHONG, &CCGWorkView::OnUpdateLightShadingPhong)
	ON_COMMAND(ID_BUTTON_BCULLING, &CCGWorkView::OnButtonBculling)
	ON_COMMAND(ID_BUTTON_SIL, &CCGWorkView::OnButtonSil)
	ON_COMMAND(ID_BUTTON_INVERSE_N, &CCGWorkView::OnButtonInverseN)
	ON_COMMAND(ID_OPTIONS_SILHOUETTEOPTIONS, &CCGWorkView::OnOptionsSilhouetteoptions)
	ON_COMMAND(ID_BACKGROUND_CLEAR, &CCGWorkView::OnBackgroundClear)
	ON_COMMAND(ID_FOGEFFECT_ENABLE, &CCGWorkView::OnFogeffectEnable)
	ON_UPDATE_COMMAND_UI(ID_FOGEFFECT_ENABLE, &CCGWorkView::OnUpdateFogeffectEnable)
	ON_COMMAND(ID_FOGEFFECT_OPTIONS, &CCGWorkView::OnFogeffectOptions)
	ON_COMMAND(ID_ANIMATION_RECORD, &CCGWorkView::OnAnimationRecord)
	ON_UPDATE_COMMAND_UI(ID_ANIMATION_RECORD, &CCGWorkView::OnUpdateAnimationRecord)
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_ANIMATION_PLAY, &CCGWorkView::OnAnimationPlay)
	ON_COMMAND(ID_ANIMATION_SAVETOIMAGES, &CCGWorkView::OnAnimationSavetoimages)
	ON_COMMAND(ID_INTERPOLATION_LINEAR, &CCGWorkView::OnInterpolationLinear)
	ON_UPDATE_COMMAND_UI(ID_INTERPOLATION_LINEAR, &CCGWorkView::OnUpdateInterpolationLinear)
	ON_COMMAND(ID_INTERPOLATION_BEZIER, &CCGWorkView::OnInterpolationBezier)
	ON_UPDATE_COMMAND_UI(ID_INTERPOLATION_BEZIER, &CCGWorkView::OnUpdateInterpolationBezier)
	ON_COMMAND(ID_PLAYBACKSPEED_NORMAL, &CCGWorkView::OnPlaybackspeedNormal)
	ON_COMMAND(ID_PLAYBACKSPEED_INCREASE, &CCGWorkView::OnPlaybackspeedIncrease)
	ON_COMMAND(ID_PLAYBACKSPEED_DECREASE, &CCGWorkView::OnPlaybackspeedDecrease)
	ON_WM_PAINT()
	ON_COMMAND(ID_ANTI_OPTIONS, &CCGWorkView::OnAntiOptions)
	ON_COMMAND(ID_ANTI_ENABLED, &CCGWorkView::OnAntiEnabled)
	ON_UPDATE_COMMAND_UI(ID_ANTI_ENABLED, &CCGWorkView::OnUpdateAntiEnabled)
	ON_COMMAND(ID_FOGEFFECT_PORTAL, &CCGWorkView::OnFogeffectPortal)
	ON_UPDATE_COMMAND_UI(ID_FOGEFFECT_PORTAL, &CCGWorkView::OnUpdateFogeffectPortal)
	ON_COMMAND(ID_FILE_CLEARALL, &CCGWorkView::OnFileClearall)
END_MESSAGE_MAP()

// A patch to fix GLaux disappearance from VS2005 to VS2008
void auxSolidCone(GLdouble radius, GLdouble height) {
        GLUquadric *quad = gluNewQuadric();
        gluQuadricDrawStyle(quad, GLU_FILL);
        gluCylinder(quad, radius, 0.0, height, 20, 20);
        gluDeleteQuadric(quad);
}

/////////////////////////////////////////////////////////////////////////////
// CCGWorkView construction/destruction

CCGWorkView::CCGWorkView()
{
	// Set default values
	m_isAxis_X = true;
	m_isAxis_Y = false;
	m_isAxis_Z = false;
	m_nAction = ID_ACTION_ROTATE;
	m_nView = ID_VIEW_ORTHOGRAPHIC;	
	m_bIsPerspective = false;

	m_nLightShading = ID_LIGHT_SHADING_FLAT;

	m_lMaterialAmbient = 0.2;
	m_lMaterialDiffuse = 0.8;
	m_lMaterialSpecular = 1.0;
	m_nMaterialCosineFactor = 32;

	//init the first light to be enabled
	m_lights[LIGHT_ID_1].Enabled = true;
	m_lights[LIGHT_ID_1].Space = LIGHT_SPACE_LOCAL;
	m_lights[LIGHT_ID_1].DirX = 1.0;
	m_lights[LIGHT_ID_1].DirY = -1.0;
	m_lights[LIGHT_ID_1].DirZ = 1.0;
	m_pDbBitMap = NULL;
	m_pDbDC = NULL;

	// Init Buffers
	m_FrameBuffer = NULL;
	zBuffer = NULL;

	isFirstDraw = true;
	isBBoxOn = false;
	isCColorDialogOpen = false;
	mouseClicked = false;
	m_sensitivity = Vec4(20, 100, 300);
	orthoHeight = 5.0;
	m_nCoordSpace = ID_BUTTON_OBJECT;
	normalSizeFactor = 0.5;
	showGeos = true;
	aroundEye = true;
	currentPolySelection = WIREFRAME;
	isBGStretch = false;
	isBGFileOpen = false;
	isModelLoaded = false;
	isBFCulling = false;
	showSil = false;
	silThickness = 3;
	silColor = AL_YELLO_CREF;
	saveToFile = false;
	
	// Animation parameters
	isRecording = false;
	isPlaying = false;
	isLinearInterpolation = true;
	framesPerSeconds = 30;
	translationOffset = Vec4(0.0);
	rotationOffset = Vec4(0.0);
	scaleOffset = Vec4(1.0);
	currentFrame = 0;
}

CCGWorkView::~CCGWorkView()
{
}


/////////////////////////////////////////////////////////////////////////////
// CCGWorkView diagnostics

#ifdef _DEBUG
void CCGWorkView::AssertValid() const
{
	CView::AssertValid();
}

void CCGWorkView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCGWorkDoc* CCGWorkView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCGWorkDoc)));
	return (CCGWorkDoc*)m_pDocument;
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CCGWorkView Window Creation - Linkage of windows to CGWork

BOOL CCGWorkView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	// An CGWork window must be created with the following
	// flags and must NOT include CS_PARENTDC for the
	// class style.

	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	return CView::PreCreateWindow(cs);
}

void CCGWorkView::DrawLine( COLORREF color, CPoint a, CPoint b, int thickness)
{
	int oct = GetOctant(a, b);
	b = TranslateBToFirstOctant(a, b, oct);

	int x, y, dx, dy, d, De, Dne;
	x = a.x;
	y = a.y;
	dx = b.x - a.x;
	dy = b.y - a.y;
	d = 2 * dy - dx;
	De = 2 * dy;
	Dne = 2 * (dy - dx);
	DrawPointOctant(x, y, color, a, oct, thickness);
	while (x < b.x) {
		if (d < 0) {
			d += De;
			x++;
		}
		else {
			d += Dne;
			x++;
			y++;
		}
		DrawPointOctant(x, y, color, a, oct, thickness);
	}
}

void CCGWorkView::DrawPoly(std::vector<Edge> edges)
{
	for (Edge e : edges) {
		DrawLine(e.color, e.A.Pixel, e.B.Pixel);
	}
}

class EdgeSorterY
{
public:
	bool operator()(const Edge& e1, const Edge& e2)
	{
		return (((e1.A.Pixel.y < e2.A.Pixel.y) && (e1.A.Pixel.y < e2.B.Pixel.y)) ||
			((e1.B.Pixel.y < e2.A.Pixel.y) && (e1.B.Pixel.y < e2.B.Pixel.y)));
	}
};

class EdgeComparer
{
public:
	bool operator()(const Edge& e1, const Edge& e2)
	{
		return ((e1.A.Pixel == e2.A.Pixel) && (e1.B.Pixel == e2.B.Pixel));
	}
};

struct Intersection {
	int x;
	double z;
	Vec4 color;
	Vec4 normal;
	Vec4 pos;
};

class IntersectionsSorter
{
public:
	bool operator()(const Intersection& i1, const Intersection& i2)
	{
		auto less = std::less<int>();
		return less(i1.x, i2.x);
	}
};

class IntersectionsComparer
{
public:
	bool operator()(const Intersection& i1, const Intersection& i2)
	{
		return (i1.x == i2.x);
	}
};

void CCGWorkView::ScanConvert(std::vector<Edge> poly, Material* material, COLORREF color, Vec4 polyCenter, Vec4 polyNormal)
{
	assert(poly.size() > 2);
	COLORREF objectColor = color;
	if (m_nLightShading == ID_LIGHT_SHADING_FLAT)
	{
		// Calculate color
		Vec4 c = CalculateShading(m_lights, material, polyCenter, polyNormal, objectColor);
		color = RGB((int)(c[0] * 255.0), (int)(c[1] * 255.0), (int)(c[2] * 255.0));

		if (fog.IsEnabled)
			color = GetColorWithFog(polyCenter, color);
	}

	// Sort edges according to the ymin value
	EdgeSorterY sorter;
	std::sort(poly.begin(), poly.end(), sorter);
	
	std::vector<Edge> activeList;

	// find ymax of edges in poly
	int ymax = poly[0].A.Pixel.y;
	for (unsigned int i = 0; i < poly.size(); i++)
	{
		if (ymax <= poly[i].A.Pixel.y)
			ymax = poly[i].A.Pixel.y;
		if (ymax <= poly[i].B.Pixel.y)
			ymax = poly[i].B.Pixel.y;
	}
	
	int ymin = max(min(poly[0].A.Pixel.y, poly[0].B.Pixel.y), 0);
	// Iterate over scan lines from ymin to ymax
	for (int y = ymin; y <= ymax; y++)
	{
		// Iterate over the edges in Poly
		//for (unsigned int i = 0; i < poly.size(); i++)
		for (auto it = poly.begin(); it != poly.end(); ++it)
		{
			Edge polyEdge = *it;
			int edgeYMin = (polyEdge.A.Pixel.y < polyEdge.B.Pixel.y) ? polyEdge.A.Pixel.y : polyEdge.B.Pixel.y;
			int edgeYMax = (polyEdge.A.Pixel.y < polyEdge.B.Pixel.y) ? polyEdge.B.Pixel.y : polyEdge.A.Pixel.y;

			if (activeList.size() == 0 && edgeYMin <= y)
			{
				activeList.push_back(polyEdge);
				continue;
			}

			// Iterate over the edges in active list
			int size = activeList.size();
			for (unsigned int i = 0; i < activeList.size(); i++)
			{
				// If the poly edge is not in the active list and it's ymin is smaller
				// than the scanline, add it to the active list
				if ((polyEdge.A.Pixel != activeList[i].A.Pixel) || (polyEdge.B.Pixel != activeList[i].B.Pixel))
				{
					if (edgeYMin <= y)
					{
						activeList.push_back(polyEdge);
						break;
					}
				}
				else
					break;
			}
			for (unsigned int i = 0; i < activeList.size(); i++)
			{
				// If the poly edge is in the active list and it's ymax is smaller
				// than the scanline, remove it from the active list
				if (((polyEdge.A.Pixel == activeList[i].A.Pixel) && (polyEdge.B.Pixel == activeList[i].B.Pixel)) &&
					(edgeYMax < y))
				{
					activeList.erase(activeList.begin() + i);
				}
			}
		}

		EdgeComparer eComp;
		auto it = std::unique(activeList.begin(), activeList.end(), eComp);
		activeList.resize(std::distance(activeList.begin(), it));

		// Calculate points of intersections of A members with line Y = y
		std::vector<Intersection> intersections;
		for (Edge e : activeList)
		{
			int dy = e.B.Pixel.y - e.A.Pixel.y;
			if (dy == 0)
				continue;
			int dx = e.B.Pixel.x - e.A.Pixel.x;

			Intersection i;
			// Calculate X axis intersections
			int x = (int)floor((y * dx + (e.A.Pixel.x * e.B.Pixel.y - e.B.Pixel.x * e.A.Pixel.y)) / dy);
			i.x = x;

			// Calculate Z pos at intersections
			double z = e.A.Z - (e.A.Z - e.B.Z) * ((double)(e.A.Pixel.y - y) / (double)(e.A.Pixel.y - e.B.Pixel.y));
			i.z = z;

			for (int j = 0; j < 3; j++)
			{
				// Calculate Color at intersections
				i.color[j] = e.A.Color[j] - (e.A.Color[j] - e.B.Color[j]) * ((double)(e.A.Pixel.y - y) / (double)(e.A.Pixel.y - e.B.Pixel.y));
				// Calculate Normal at intersections
				i.normal[j] = e.A.NormalVS[j] - (e.A.NormalVS[j] - e.B.NormalVS[j]) * ((double)(e.A.Pixel.y - y) / (double)(e.A.Pixel.y - e.B.Pixel.y));
				// Calculate Position at intersections
				i.pos[j] = e.A.PosVS[j] - (e.A.PosVS[j] - e.B.PosVS[j]) * ((double)(e.A.Pixel.y - y) / (double)(e.A.Pixel.y - e.B.Pixel.y));
			}

			intersections.push_back(i);
		}

		// Sort intersections by decreasing x values
		IntersectionsSorter sorter;
		std::sort(intersections.begin(), intersections.end(), sorter);
		
		IntersectionsComparer comp;
		auto it2 = std::unique(intersections.begin(), intersections.end(), comp);
		intersections.resize(std::distance(intersections.begin(), it2));
		
		if (intersections.size() == 1)
		{
			// Draw pixel
		}
		// Draw line according to shading and zbuffer
		for (unsigned int i = 0; i < intersections.size(); i += 2)
		{
			// Get intersections x pixel pos
			int x0 = intersections[i].x;
			if ((i + 1) >= intersections.size())
				break;
			int x1 = intersections[i + 1].x;

			// Get intersections z pos
			double z0 = intersections[i].z;
			if ((i + 1) >= intersections.size())
				break;
			double z1 = intersections[i + 1].z;

			// Get intersections RGB
			Vec4 I0 = intersections[i].color;
			if ((i + 1) >= intersections.size())
				break;
			Vec4 I1 = intersections[i + 1].color;

			// Get intersections Normal
			Vec4 n0 = intersections[i].normal;
			if ((i + 1) >= intersections.size())
				break;
			Vec4 n1 = intersections[i + 1].normal;

			// Get intersections Pos
			Vec4 v0 = intersections[i].pos;
			if ((i + 1) >= intersections.size())
				break;
			Vec4 v1 = intersections[i + 1].pos;

			int x = x0;
			while (x <= x1)
			{
				// Caluclate zPos and pos at (x, y)
				double zp = z1 - (z1 - z0) * ((double)(x1 - x) / (double)(x1 - x0));
				Vec4 pos = v1 - (v1 - v0) * ((double)(x1 - x) / (double)(x1 - x0));

				// Calcualte RGB at (x, y) according to shading
				if (m_nLightShading == ID_LIGHT_SHADING_GOURAUD)
				{
					Vec4 colorVec = I1 - (I1 - I0) * ((double)(x1 - x) / (double)(x1 - x0));
					color = RGB((int)colorVec[0], (int)colorVec[1], (int)colorVec[2]);
				}
				if (m_nLightShading == ID_LIGHT_SHADING_PHONG)
				{
					Vec4 normal = n1 - (n1 - n0) * ((double)(x1 - x) / (double)(x1 - x0));
					// Calculate color
					Vec4 c = CalculateShading(m_lights, material, pos, normal, objectColor);
					color = RGB((int)(c[0] * 255.0), (int)(c[1] * 255.0), (int)(c[2] * 255.0));
				}
				// Compare z Pos to zBuffer, if z Pos > zBuffer,
				// Draw and update z buffer

				int index = max(0, min(x + m_Width * y, m_Width * m_Height - 1));
				if (zp > zBuffer[index])
				{
					if (fog.IsEnabled && (m_nLightShading != ID_LIGHT_SHADING_FLAT))
						color = GetColorWithFog(pos, color);

					SetPixelBuffer(x, y, color);
					zBuffer[index] = zp;
				}
				x++;
			}
		}
	}
}


int CCGWorkView::GetOctant(CPoint a, CPoint b)
{
	//normalize at b as if a is in (0,0)
	b.x -= a.x;
	b.y -= a.y;

	//problematic areas
	if (b == CPoint(0, 0)) return 1;
	if (b.x == 0) return b.y > 0 ? 7 : 3;
	if (b.y == 0) return b.x > 0 ? 1 : 5;

	//Right half
	if (b.x > 0) {
		//I
		if (b.y < 0) {
			return b.x > abs(b.y) ? 1 : 2;
		}
		//IV
		else {
			return b.y > b.x ? 7 : 8;
		}
	}

	//Left half
	else {
		//II
		if (b.y < 0) {
			return b.x > b.y ? 3 : 4;
		}
		//III
		else
		{
			return abs(b.x) > b.y ? 5 : 6;
		}
	}
}

void CCGWorkView::DrawPointOctant(int x, int y, COLORREF color, const CPoint& origA, int oct, int thickness)
{
	CPoint normalized = CPoint(x - origA.x, y - origA.y);
	CPoint transformed = TranslatePointFrom8th(normalized, oct);
	CPoint actualPoint = origA + transformed;

	if (thickness == 0)
		SetPixelBuffer(actualPoint.x, actualPoint.y, color);
	else
	{
		// Draw thickness
		int startX = actualPoint.x;
		int endX = actualPoint.x;
		int startY = actualPoint.y;
		int endY = actualPoint.y;
		for (int i = thickness; i > 0; i--)
		{
			if ((startX == actualPoint.x) && (actualPoint.x - i >= 0))
				startX = actualPoint.x - i;
			if ((endX == actualPoint.x) && (actualPoint.x + i < m_Width))
				endX = actualPoint.x + i;
			if ((startY == actualPoint.y) && (actualPoint.y - i >= 0))
				startY = actualPoint.y - i;
			if ((endY == actualPoint.y) && (actualPoint.y + i < m_Height))
				endY = actualPoint.y + i;
		}

		CPoint pix;
		for (int x = startX; x <= endX; x++)
		{
			for (int y = startY; y <= endY; y++)
			{
				SetPixelBuffer(x, y, color);
			}
		}
	}
}

CPoint CCGWorkView::TranslateBToFirstOctant(const CPoint & a, const CPoint & b, int oct)
{
	assert(0 < oct && oct <= 8);
	CPoint normalized = b - a;
	CPoint translated = TranslatePointTo8th(normalized, oct);
	return a + translated;
}

//Translates 8th octant point to given octant
CPoint CCGWorkView::TranslatePointFrom8th(CPoint p, int oct)
{
	assert(0 < oct && oct <= 8);

	int x, y;
	switch (oct)
	{
	case 1: x = p.x;	y = -p.y;	break;
	case 2: x = p.y;	y = -p.x;	break;
	case 3: x = -p.y;	y = -p.x;	break;
	case 4: x = -p.x;	y = -p.y;	break;
	case 5: x = -p.x;	y = p.y;	break;
	case 6: x = -p.y;	y = p.x;	break;
	case 7: x = p.y;	y = p.x;	break;
	case 8: x = p.x;	y = p.y;	break;
	default:
		break;
	}
	return CPoint(x, y);
}

CPoint CCGWorkView::TranslatePointTo8th(CPoint p, int oct)
{
	assert(0 < oct && oct <= 8);

	//Translation is identical except 2nd and 6th octants
	if (oct == 2 || oct == 6) {
		p.x *= -1;
		p.y *= -1;
	}

	return TranslatePointFrom8th(p, oct);
}

void CCGWorkView::SetSelectedPoly(CPoint mousePos, Poly* p, std::vector<Vec4Line> poly)
{
	std::vector<double> point;
	point.push_back(mousePos.x);
	point.push_back(mousePos.y);

	if (PointPolyIntersection(point, poly))
		p->IsSelected = true;
	else
		p->IsSelected = false;
}

void CCGWorkView::DrawSelectedPolys()
{
	for (std::vector<Edge>& poly : selectedPolys)
	{
		for (Edge& edge : poly)
		{
			edge.color = AL_YELLO_CREF;
		}
		DrawPoly(poly);
	}
	selectedPolys.clear();
}

void CCGWorkView::DrawBoundingBox(const std::vector<Poly*>& polys, const Mat4& objToWorld, 
	const Mat4& camTransform, const Mat4& viewTransform, const Mat4& projection, const Mat4& toView, COLORREF color)
{
	for (Poly* p : polys)
	{
		std::vector<Edge> poly;
		for (unsigned int i = 0; i < p->Vertices.size(); i++)
		{
			Vec4 pos1 = p->Vertices[i]->Pos;
			Vec4 pos2 = p->Vertices[(i + 1) % p->Vertices.size()]->Pos;

			Vec4 clipped1 = pos1 * objToWorld * camTransform * viewTransform * projection;
			Vec4 clipped2 = pos2 * objToWorld * camTransform * viewTransform * projection;

			clipped1 /= clipped1[3];
			clipped2 /= clipped2[3];

			Vec4 pix1Vec(clipped1 * toView);
			Vec4 pix2Vec(clipped2 * toView);

			CPoint pix1((int)pix1Vec[0], (int)pix1Vec[1]);
			CPoint pix2((int)pix2Vec[0], (int)pix2Vec[1]);

			DVertex dv1, dv2;
			dv1.Pixel = pix1;
			dv2.Pixel = pix2;

			poly.push_back({ dv1, dv2 , color });
		}

		DrawPoly(poly);
	}
}

void CCGWorkView::DrawVertexNormal(const Vec4& vertPosVS, const Vec4& normalVS, 
	const Mat4 & projection, const Mat4 & toView, COLORREF color)
{
	Vec4 startPos = vertPosVS;
	Vec4 normal = Vec4::Normalize3(normalVS);
	Vec4 endPos = startPos + normal * normalSizeFactor;

	// Transform Start and End Pos to screen space
	startPos = startPos * projection;
	startPos /= startPos[3];
	startPos = startPos * toView;

	endPos = endPos * projection;
	endPos /= endPos[3];
	endPos = endPos * toView;

	// Draw normal
	CPoint startPosPix((int)startPos[0], (int)startPos[1]);
	CPoint endPosPix((int)endPos[0], (int)endPos[1]);
	DrawLine(color, startPosPix, endPosPix);
}

void CCGWorkView::DrawPolyNormal(const Vec4& polyCenterVS, const Vec4& normalVS, 
	const Mat4& projection, const Mat4& toView, COLORREF color)
{
	Vec4 polyCenter = polyCenterVS;
	Vec4 normal = normalVS;
	normal = Vec4::Normalize3(normal);

	// Calculate endPos
	Vec4 endPos = polyCenter + normal * normalSizeFactor;

	// Transform PolyCenter and End Pos to screen space
	polyCenter = polyCenter * projection;
	polyCenter /= polyCenter[3];
	polyCenter = polyCenter * toView;

	endPos = endPos * projection;
	endPos /= endPos[3];
	endPos = endPos * toView;

	// Draw normal
	CPoint polyCenterPix((int)polyCenter[0], (int)polyCenter[1]);
	CPoint endPosPix((int)endPos[0], (int)endPos[1]);
	DrawLine(color, polyCenterPix, endPosPix);
}

void CCGWorkView::DrawBackground()
{
	if (((currentPolySelection != WIREFRAME) || saveToFile) && isBGFileOpen && !fog.IsEnabled)
	{
		CT2A BG(BGFile);
		PngWrapper pngReadFile(BG);
		pngReadFile.ReadPng();

		if (isBGStretch) {
			// Scale factors
			double cx = (double)m_Width / (double)pngReadFile.GetWidth();
			double cy = (double)m_Height / (double)pngReadFile.GetHeight();

			for (int x = 0; x < m_Width; x++) {
				for (int y = 0; y < m_Height; y++) {
					// Calculate position in image
					double v = x / cx;
					double w = y / cy;

					// Pick the nearest neighbor to (v,w)
					int c = pngReadFile.GetValue((int)round(v), (int)round(w));
					int r = GET_R(c);
					int g = GET_G(c);
					int b = GET_B(c);
					SetPixelBuffer(x, y, RGB(r, g, b));
				}
			}
		}
		else {
			for (int x = 0; x < m_Width; x++) {
				for (int y = 0; y < m_Height; y++) {
					int c = pngReadFile.GetValue(x % pngReadFile.GetWidth(), y % pngReadFile.GetHeight());
					int r = GET_R(c);
					int g = GET_G(c);
					int b = GET_B(c);

					SetPixelBuffer(x, y, RGB(r, g, b));
				}
			}
		}
	}
	else {
		COLORREF bGColorRef = m_colorDialog.BackgroundColor;
		if ((fog.IsEnabled && currentPolySelection != WIREFRAME) || (fog.IsEnabled && saveToFile))
			bGColorRef = RGB(fog.Color[0], fog.Color[1], fog.Color[2]);

		for (int x = 0; x < m_Width; x++)
			for (int y = 0; y < m_Height; y++)
				SetPixelBuffer(x, y, bGColorRef);			
		
	}
}

void CCGWorkView::DrawSilhouetteEdges(Model* model, Geometry* geo, const Mat4& objToWorld, const Mat4& camTransform,
	const Mat4& viewTransform, const Mat4& projection, const Mat4& toView, COLORREF color)
{
	for (PolyEdge* e : geo->Edges)
	{
		if (IsSilhouetteEdge(model, e, objToWorld, camTransform, viewTransform, projection))
		{
			// Transform from object space to projected space
			Vec4 p1 = e->A->Pos * objToWorld * camTransform * viewTransform * projection;
			Vec4 p2 = e->B->Pos * objToWorld * camTransform * viewTransform * projection;
			p1 /= p1[3];
			p2 /= p2[3];

			// Transform to screen space
			p1 = p1 * toView;
			p2 = p2 * toView;

			// Draw line
			CPoint p1Pix((int)p1[0], (int)p1[1]);
			CPoint p2Pix((int)p2[0], (int)p2[1]);
			DrawLine(color, p1Pix, p2Pix, silThickness - 1); // converting thickness to be 0-2 from 1-3
		}
	}
}

void CCGWorkView::SetPixelBuffer(int x, int y, COLORREF color)
{	
	if (x < 0 || x >= m_Width || y < 0 || y >= m_Height) return;

	int yCor = (m_Height - 1) - y;
	int pos = x + yCor * m_Width;

	COLORREF b = GetRValue(color)<<16;
	COLORREF g = GetGValue(color)<<8;
	COLORREF r = GetBValue(color);

	COLORREF correctedColor = b|g|r;
	m_FrameBuffer[pos] = correctedColor;
}

int CCGWorkView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	InitializeCGWork();

	return 0;
}

// This method initialized the CGWork system.
BOOL CCGWorkView::InitializeCGWork()
{
	m_pDC = new CClientDC(this);
	
	if ( NULL == m_pDC ) { // failure to get DC
		::AfxMessageBox(CString("Couldn't get a valid DC."));
		return FALSE;
	}

	CRect r;
	GetClientRect(&r);
	m_pDbDC = new CDC();
	m_pDbDC->CreateCompatibleDC(m_pDC);
	SetTimer(1, (int)((1.0 / framesPerSeconds) * 1000), NULL);
	m_pDbBitMap = CreateCompatibleBitmap(m_pDC->m_hDC, r.right, r.bottom);	
	m_pDbDC->SelectObject(m_pDbBitMap);

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CCGWorkView message handlers


void CCGWorkView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);

	if ( 0 >= cx || 0 >= cy ) {
		return;
	}

	// save the width and height of the current window
	m_WindowWidth = cx;
	m_WindowHeight = cy;

	// compute the aspect ratio
	// this will keep all dimension scales equal
	m_AspectRatio = (GLdouble)m_WindowWidth/(GLdouble)m_WindowHeight;

	CRect r;
	GetClientRect(&r);
	DeleteObject(m_pDbBitMap);
	m_pDbBitMap = CreateCompatibleBitmap(m_pDC->m_hDC, r.right, r.bottom);	
	m_pDbDC->SelectObject(m_pDbBitMap);

	// Update camera projections
	Camera* camera = Scene::GetInstance().GetCamera();
	bool isPerspective = camera->IsPerspective();
	OrthographicParams oParams = camera->GetOrthographicParameters();
	PerspectiveParams pParams = camera->GetPerspectiveParameters();
#ifdef D_PERSP
	camera->SetPerspective2(pParams.FOV, m_AspectRatio, pParams.Near, pParams.Far);
#else
	camera->SetPerspective(pParams.FOV, m_AspectRatio, pParams.Near, pParams.Far);
#endif
	double width = m_AspectRatio * orthoHeight;
	Scene::GetInstance().GetCamera()->SetOrthographic(-width / 2.0,
		width / 2.0, orthoHeight / 2.0, -orthoHeight / 2.0, oParams.Near, oParams.Far);
	camera->SwitchProjection(isPerspective);
}


BOOL CCGWorkView::SetupViewingFrustum(void)
{
    return TRUE;
}


// This viewing projection gives us a constant aspect ration. This is done by
// increasing the corresponding size of the ortho cube.
BOOL CCGWorkView::SetupViewingOrthoConstAspect(void)
{
	return TRUE;
}

Vec4 CCGWorkView::CalculateShading(LightParams* lights, Material* material, Vec4 pos, Vec4 normal, COLORREF color)
{
	Vec4 ambient = (Vec4(m_ambientLight.ColorR, m_ambientLight.ColorG, m_ambientLight.ColorB) / 255.0) * material->Ka;
	Vec4 diffuse;
	Vec4 specular;
	int numEnabledLights = 0;
	for (int i = 0; i < MAX_LIGHT; i++)
	{
		if (lights[i].Enabled)
		{
			Vec4 lightPos(lights[i].PosX, lights[i].PosY, lights[i].PosZ);
			Vec4 direction(lights[i].DirX, lights[i].DirY, lights[i].DirZ);
			if (lights[i].Space == LIGHT_SPACE_LOCAL)
			{
				Mat4 camTransform = Scene::GetInstance().GetCamera()->GetTranform();

				lightPos = lightPos * camTransform;

				camTransform[3][0] = 0.0;
				camTransform[3][1] = 0.0;
				camTransform[3][2] = 0.0;
				direction = direction * camTransform;
			}
			if (lights[i].Type == LIGHT_TYPE_POINT)
				direction = pos - lightPos;

			direction = -direction;

			Vec4 intensity(lights[i].ColorR, lights[i].ColorG, lights[i].ColorB);
			intensity /= 255.0;	// So intensity will be between 0.0 - 1.0

			// normalize everything
			direction = Vec4::Normalize3(direction);
			normal = Vec4::Normalize3(normal);

			Vec4 camPos(0.0);
			if (!m_bIsPerspective)
			{
				camPos[2] = 1.0;
			}

			// Diffuse calculation
			Vec4 diffuseCoeff = material->Kd * max(Vec4::Dot3(-normal, -direction), 0.0);
			Vec4 diffuseSingle = intensity * diffuseCoeff;

			// Specular calculation
			Vec4 camDir = Vec4::Normalize3(camPos - pos);
			Vec4 R = normal * 2;
			R = R * max(Vec4::Dot3(-normal, -direction), 0.0);
			R = R - direction;
			R = Vec4::Normalize3(R);
			Vec4 specularCoeff = material->Ks * pow(max(Vec4::Dot3(R, camDir), 0.0), material->Specular);
			Vec4 specularSingle = intensity * specularCoeff;

			if (lights[i].Type == LIGHT_TYPE_SPOT)
			{
				Vec4 directioToPos = Vec4::Normalize3(pos - lightPos);
				double theta = Vec4::Dot3(directioToPos, -direction);
				double outerCutoff = cos(ToRadians(lights[i].OuterConeAngle));

				if (lights[i].SoftSpot)
				{
					double innerCutoff = cos(ToRadians(lights[i].InnerConeAngle));
					double epsilon = innerCutoff - outerCutoff;
					double intensity = ClampDbl((theta - outerCutoff) / epsilon, 0.0, 1.0);

					diffuseSingle *= intensity;
					specularSingle *= intensity;
				}
				else
				{
					if (theta <= outerCutoff)
					{
						diffuseSingle = Vec4(0.0);
						specularSingle = Vec4(0.0);
					}
				}
			}

			if (lights[i].Type != LIGHT_TYPE_DIRECTIONAL)
			{
				double distance = Vec4::Distance3(pos, lightPos);
				if (lights[i].Attenuation == LIGHT_ATTENUATION_NONE)
					distance = 1.0;
				if (lights[i].Attenuation == LIGHT_ATTENUATION_POWEROF2)
					distance *= distance;

				diffuseSingle /= distance;
				specularSingle /= distance;
			}

			diffuse += diffuseSingle;
			specular += specularSingle;

			numEnabledLights++;
		}
	}

	// Normalize diffuse and specular by number of lights
	/*
	if (numEnabledLights > 0)
	{
		diffuse /= numEnabledLights;
		specular /= numEnabledLights;
	}
	*/

	Vec4 objectColor =  Vec4(GetRValue(color) / 255.0, GetGValue(color) / 255.0, GetBValue(color) / 255.0);
	Vec4 sum = (ambient + diffuse) * objectColor + specular;
	return Vec4(min(sum[0], 1.0), min(sum[1], 1.0), min(sum[2], 1.0));
}

bool CCGWorkView::IsBackFace(Model* model, const Poly* p, const Mat4& objToWorld,
	const Mat4& camTransform, const Mat4& viewTransform, const Mat4& projection)
{
	Vec4 normal = Scene::GetInstance().GetCalcNormalState() ?
		CalculatePolyNormal(p, objToWorld, camTransform, viewTransform) :
		p->Normal * objToWorld * camTransform * viewTransform;
	normal *= model->NormalSign;
	normal = Vec4::Normalize3(normal);

	// Transform poly center to View Space
	Vec4 center = p->Center * objToWorld * camTransform * viewTransform;

	if (m_bIsPerspective)
	{
		if (Vec4::Dot3(center, normal) > 0)
			return true;
		return false;
	}

	normal = Vec4::Normalize3(normal * projection);
	return normal[2] < 0;
}

bool CCGWorkView::IsSilhouetteEdge(Model* model, const PolyEdge* e, const Mat4& objToWorld, const Mat4& camTransform, 
	const Mat4& viewTransform, const Mat4& projection)
{
	if (e->Polys.size() != 2)
		return false;

	bool isPoly1BF = IsBackFace(model, e->Polys[0], objToWorld, camTransform, viewTransform, projection);
	bool isPoly2BF = IsBackFace(model, e->Polys[1], objToWorld, camTransform, viewTransform, projection);
	return (isPoly1BF ^ isPoly2BF);
}

Vec4 CCGWorkView::CalculatePolyNormal(const Poly * p, const Mat4& objToWorld, 
	const Mat4& camTransform, const Mat4& viewTransform)
{
	Vec4 normal;
	if (p->Vertices.size() > 3)
	{
		Vec4 u = p->Vertices[0]->Pos * objToWorld * camTransform * viewTransform;
		Vec4 v = p->Vertices[1]->Pos * objToWorld * camTransform * viewTransform;
		Vec4 w = p->Vertices[2]->Pos * objToWorld * camTransform * viewTransform;

		Vec4 norm = Vec4::Cross(u - v, v - w);
		normal += Vec4::Normalize3(norm);
	}
	else
		normal += p->Normal * objToWorld * camTransform * viewTransform;

	return normal;
}

Vec4 CCGWorkView::CalculateVertexNormal(const Vertex* v, const Mat4& objToWorld, const Mat4& camTransform,
	const Mat4& viewTransform)
{
	Vec4 normal;
	for (Poly* p : v->Polys)
		normal += CalculatePolyNormal(p, objToWorld, camTransform, viewTransform);

	normal /= v->Polys.size();
	normal = Vec4::Normalize3(normal);
	normal[3] = 0.0;

	return normal;
}

COLORREF CCGWorkView::GetColorWithFog(const Vec4& posVS, COLORREF objColor)
{
	double factor = 0.0;
	double dist = Vec4::Length3(posVS);

	if (fog.BlendingFunction == FOG_BLENDING_LINEAR)
	{
		if (dist <= fog.MinFogDistance)
			factor = 0.0;
		else if ((dist < fog.MaxFogDistance) && (dist > fog.MinFogDistance))
		{
			factor = 1.0 - ((fog.MaxFogDistance - dist) / (fog.MaxFogDistance - fog.MinFogDistance));

			if (fog.IsPortal)
				factor = (factor <= fog.PortalThreshold) ? 0.0 : ClampDbl(factor, fog.PortalThreshold, 1.0);
			else
				factor = ClampDbl(factor, 0.0, 1.0);
		}
		else
			factor = 1.0;
	}
	else if (fog.BlendingFunction == FOG_BLENDING_EXP)
	{
		factor = 1.0 - exp(-fog.Exponent * dist);
	}
	else
	{
		factor = 1.0 - exp(-pow(fog.Exponent * dist, 2));
	}

	int objR = GetRValue(objColor);
	int objG = GetGValue(objColor);
	int objB = GetBValue(objColor);

	int finalR = (int)(factor * fog.Color[0] + (1.0 - factor) * objR);
	int finalG = (int)(factor * fog.Color[1] + (1.0 - factor) * objG);
	int finalB = (int)(factor * fog.Color[2] + (1.0 - factor) * objB);

	return RGB(finalR, finalG, finalB);
}

void CCGWorkView::WriteToStatusBar(const CString & str)
{
	CStatusBar* p =
		(CStatusBar*)AfxGetApp()->m_pMainWnd->GetDescendantWindow(AFX_IDW_STATUS_BAR);
	p->SetPaneText(0, str);
}

COLORREF CCGWorkView::Vec4ToColor(const Vec4 & c) const
{
	return RGB((int)c[0], (int)c[1], (int)c[2]);
}

void CCGWorkView::AddFirstFrame(Model* model)
{
	if (isRecording)
	{
		Animation* anim = model->GetAnimation();
		anim->ClearAnimation();
		
		// Add key frame
		Frame* keyFrame = new Frame();
		keyFrame->ObjToWorld = model->GetObjToWorldTransform();
		keyFrame->ViewTransform = model->GetViewTransform();
		keyFrame->FrameNumber = 0;

		anim->AddKeyFrame(keyFrame);
	}
}

BOOL CCGWorkView::OnEraseBkgnd(CDC* pDC) 
{
	// Windows will clear the window with the background color every time your window 
	// is redrawn, and then CGWork will clear the viewport with its own background color.

	// Clear zBuffer
	if (zBuffer != NULL)
	{
		delete[] zBuffer;
		zBuffer = NULL;
	}
	
	if (m_FrameBuffer != NULL)
	{
		delete[] m_FrameBuffer;
		m_FrameBuffer = NULL;
	}

	return true;
}



/////////////////////////////////////////////////////////////////////////////
// CCGWorkView drawing
/////////////////////////////////////////////////////////////////////////////

void CCGWorkView::OnPaint()
{
	CRect r;
	GetClientRect(&r);
	m_Width = (saveToFile) ? imgWidth : r.Width();
	m_Height = (saveToFile) ? imgHeight : r.Height();
	double aspectRatio = (double)m_Width / (double)m_Height;

	int aaSizeFactor = 1;
	if (m_antiAliasing.IsEnabled && (m_antiAliasing.Size == AA_3X3))
		aaSizeFactor = 3;
	if (m_antiAliasing.IsEnabled && (m_antiAliasing.Size == AA_5X5))
		aaSizeFactor = 5;

	m_Width *= aaSizeFactor;
	m_Height *= aaSizeFactor;

	//frameBuffer 
	CPaintDC hdc(this); // device context for painting
	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP bm = CreateCompatibleBitmap(hdc, m_Width, m_Height);
	SelectObject(hdcMem, bm);

	m_FrameBuffer = new int[m_Width * m_Height];

	if (isFirstDraw)
	{
		// Set initial Projection matricies
		double width = orthoHeight * aspectRatio;
		Scene::GetInstance().GetCamera()->SetPerspective(45.0, aspectRatio, 1.0, 1000.0);
		Scene::GetInstance().GetCamera()->SetOrthographic(orthoHeight, aspectRatio, 1, 1000.0);

		// Set Perspective dialog default values
		m_perspDialog.ProjPlane = Scene::GetInstance().GetCamera()->GetPerspectiveParameters().Near;
		m_perspDialog.FOV = Scene::GetInstance().GetCamera()->GetPerspectiveParameters().FOV;

		isFirstDraw = false;
	}

	// Initialize zBuffer
	int bufferSize = m_Width * m_Height;
	zBuffer = new double[bufferSize];
	for (int i = 0; i < bufferSize; i++)
		zBuffer[i] = -DBL_MAX;

	// Background Drawing
	DrawBackground();

	// Don't draw if model is not finished loading
	if (isModelLoaded)
	{
		std::vector<Model*> models = Scene::GetInstance().GetModels();
		Camera* camera = Scene::GetInstance().GetCamera();
		Mat4 camTransform = camera->GetTranform();
		Mat4 toView(Vec4(m_Width / 2.0, 0.0, 0.0, 0.0),
					Vec4(0.0, m_Height / 2.0, 0.0, 0.0),
					Vec4(0.0, 0.0, 1.0, 0.0),
					Vec4((m_Width - 1) / 2.0, (m_Height - 1) / 2.0, 0.0, 1.0));

		// Update camera projections
		bool isPerspective = camera->IsPerspective();
		OrthographicParams oParams = camera->GetOrthographicParameters();
		PerspectiveParams pParams = camera->GetPerspectiveParameters();
		camera->SetPerspective2(pParams.FOV, aspectRatio, pParams.Near, pParams.Far);
		double orthoWidth = aspectRatio * orthoHeight;
		camera->SetOrthographic(-orthoWidth / 2.0,
			orthoWidth / 2.0, orthoHeight / 2.0, -orthoHeight / 2.0, oParams.Near, oParams.Far);
		camera->SwitchProjection(isPerspective);
		Mat4 projection = camera->GetProjection();

		for (Model* model : models)
		{
			Mat4 objToWorld = model->GetObjToWorldTransform();
			Mat4 viewTransform = model->GetViewTransform();
			COLORREF color = isCColorDialogOpen ? m_colorDialog.WireframeColor :
				Vec4ToColor(model->GetColor());
			COLORREF normalColor = isCColorDialogOpen ? m_colorDialog.NormalColor :
				Vec4ToColor(model->GetNormalColor());
			Material* material = model->GetMaterial();

			// Replace Model and Cam transform matricies if we are playing an animation
			if (isPlaying)
			{
				const Frame* currentFrame = model->GetAnimation()->GetCurrentFrame();
				if (currentFrame != nullptr)
				{
					objToWorld = currentFrame->ObjToWorld;
					viewTransform = currentFrame->ViewTransform;
				}
			}

			for (Geometry* geo : model->GetGeometries())
			{
				color = (!showGeos) ? color :
					RGB((BYTE)geo->Color[0], (BYTE)geo->Color[1], (BYTE)geo->Color[2]);
				COLORREF selectedColor = RGB((BYTE)(255.0 - model->GetColor()[0]),
					(BYTE)(255.0 - model->GetColor()[1]), (BYTE)(255.0 - model->GetColor()[2]));
				// Draw Polys
				std::vector<Poly*> polygons = geo->Polygons;
				for (Poly* p : polygons)
				{
					std::vector<Edge> poly;
					std::vector<Vec4Line> polyEdges;

					// Check Backface culling, and draw only if it is front facing
					if (isBFCulling && IsBackFace(model, p, objToWorld, camTransform, viewTransform, projection))
						continue;

					for (unsigned int i = 0; i < p->Vertices.size(); i++)
					{
						// Save vertices in object space
						Vec4 pos1 = p->Vertices[i]->Pos;
						Vec4 pos2 = p->Vertices[(i + 1) % p->Vertices.size()]->Pos;

						// Transform vertices from object space to normalized device coords
						Vec4 clipped1 = pos1 * objToWorld * camTransform * viewTransform * projection;
						Vec4 clipped2 = pos2 * objToWorld * camTransform * viewTransform * projection;

						// Divide by W
						clipped1 /= clipped1[3];
						clipped2 /= clipped2[3];

						// Transform from NDC to screen space
						Vec4 pix1Vec(clipped1 * toView);
						Vec4 pix2Vec(clipped2 * toView);

						CPoint pix1((int)pix1Vec[0], (int)pix1Vec[1]);
						CPoint pix2((int)pix2Vec[0], (int)pix2Vec[1]);

						// Contruct poly for mouse selection test
						polyEdges.push_back({ pix1Vec, pix2Vec });

						// Set model to be selected color if the model is selected
						if (model == Scene::GetInstance().GetSelectedModel())
							color = AL_YELLO_CREF;

						// Initialize vertex for drawing
						DVertex dVertex1, dVertex2;

						// Get vertex normal from file and transform it to view space
						// Or calculate it and get it already in view space
						Vec4 normal1VS = Scene::GetInstance().GetCalcNormalState() ?
							CalculateVertexNormal(p->Vertices[i], objToWorld, camTransform, viewTransform) :
							p->Vertices[i]->Normal * objToWorld * camTransform * viewTransform;
						normal1VS *= model->NormalSign;
						Vec4 normal2VS = Scene::GetInstance().GetCalcNormalState() ?
							CalculateVertexNormal(p->Vertices[(i + 1) % p->Vertices.size()], objToWorld, camTransform, viewTransform) :
							p->Vertices[(i + 1) % p->Vertices.size()]->Normal * objToWorld * camTransform * viewTransform;
						normal2VS *= model->NormalSign;

						// Save parameters in DVertex
						dVertex1.Pixel = pix1;
						dVertex1.PosVS = pos1 * objToWorld * camTransform * viewTransform;
						dVertex1.Z = clipped1[2];
						dVertex1.NormalVS = normal1VS;
						// Calculate color
						dVertex1.Color = CalculateShading(m_lights, material, dVertex1.PosVS, dVertex1.NormalVS, color) * 255.0;

						dVertex2.Pixel = pix2;
						dVertex2.PosVS = pos2 * objToWorld * camTransform * viewTransform;
						dVertex2.Z = clipped2[2];
						dVertex2.NormalVS = normal2VS;
						// Calculate color
						dVertex2.Color = CalculateShading(m_lights, material, dVertex2.PosVS, dVertex2.NormalVS, color) * 255.0;

						// Construct poly for drawing
						if (m_colorDialog.IsDiscoMode)
							color = AL_RAINBOW_CREF;
						poly.push_back({ dVertex1, dVertex2 , color });

						// Draw vertex normal if needed
						if (Scene::GetInstance().AreVertexNormalsOn())
						{
							DrawVertexNormal(dVertex1.PosVS, normal1VS,
								projection, toView, normalColor);
						}
					}

					// If mouse button was clicked in Select mode perform intersection caculation
					if (mouseClicked)
						SetSelectedPoly(mouseClickPos, p, polyEdges);

					// If the poly was selected, save it in the selectedPolys list
					if (p->IsSelected)
						selectedPolys.push_back(poly);

					Vec4 polyCenter = p->Center * objToWorld * camTransform * viewTransform;
					Vec4 normal = Scene::GetInstance().GetCalcNormalState() ?
						CalculatePolyNormal(p, objToWorld, camTransform, viewTransform) :
						p->Normal * objToWorld * camTransform * viewTransform;
					normal *= model->NormalSign; // Poly normal in View Space coordinates

					// Draw poly in wireframe mode or fill it, according to user selection
					if (currentPolySelection == WIREFRAME && !saveToFile)
						DrawPoly(poly);
					else if (currentPolySelection == SOLID_SCREEN || saveToFile)
						ScanConvert(poly, material, color, polyCenter, normal);

					// Draw poly normal if needed
					if (Scene::GetInstance().ArePolyNormalsOn())
					{
						DrawPolyNormal(polyCenter, normal, projection, toView, normalColor);
					}
				}

				// Draw Geo Bounding Box if needed
				if (Scene::GetInstance().GetBBoxState() && showGeos)
				{
					DrawBoundingBox(geo->BBox, objToWorld, camTransform, viewTransform, 
						projection, toView, color);
				}

				// Draw Silhouette Edges if needed
				if (showSil)
				{
					DrawSilhouetteEdges(model, geo, objToWorld, camTransform, viewTransform,
						projection, toView, silColor);
				}
			}

			// Reset mouseclicked flag
			mouseClicked = false;

			// Draw the selected polys in the end to make sure they are on top
			DrawSelectedPolys();

			// Draw Model Bounding Box if needed
			if (Scene::GetInstance().GetBBoxState() && !showGeos)
			{
				DrawBoundingBox(model->GetBBox(), objToWorld, camTransform, viewTransform, 
					projection, toView, color);
			}
		}
	}

	int width = (saveToFile) ? imgWidth : m_WindowWidth;
	int height = (saveToFile) ? imgHeight : m_WindowHeight;
	
	//Anti-Aliasing
	m_FrameBuffer = m_antiAliasing.PrepareForPrint(m_FrameBuffer, width, height);

	if (saveToFile)
	{
		for (int x = 0; x < width; x++)
		{
			for (int y = 0; y < height; y++)
			{
				int yCor = (height - 1) - y;
				int index = x + width * yCor;

				COLORREF color = m_FrameBuffer[index];

				COLORREF b = GetRValue(color) << 8;
				COLORREF g = GetGValue(color) << 16;
				COLORREF r = GetBValue(color) << 24;

				COLORREF correctedColor = b | g | r;

				imgToSave.SetValue(x, y, correctedColor);
			}
		}
		imgToSave.WritePng();

		if (!isPlaying)
		{
			AfxMessageBox(L"Rendered to file successfully", MB_OK | MB_ICONINFORMATION);
			saveToFile = false;
		}
		else
		{
			std::stringstream ss;
			ss << "Saved frame " << currentFrame;
			WriteToStatusBar(ss.str().c_str());
		}
	}
	else
	{
		BITMAPINFO bminfo;
		bminfo.bmiHeader.biSize = sizeof(bminfo.bmiHeader);
		bminfo.bmiHeader.biWidth = width;
		bminfo.bmiHeader.biHeight = height;
		bminfo.bmiHeader.biPlanes = 1;
		bminfo.bmiHeader.biBitCount = 32;
		bminfo.bmiHeader.biCompression = BI_RGB;
		bminfo.bmiHeader.biSizeImage = 0;
		bminfo.bmiHeader.biXPelsPerMeter = 1;
		bminfo.bmiHeader.biYPelsPerMeter = 1;
		bminfo.bmiHeader.biClrUsed = 0;
		bminfo.bmiHeader.biClrImportant = 0;

		SetDIBits(hdcMem, bm, 0, r.bottom - r.top, m_FrameBuffer, &bminfo, 0);
		BitBlt(hdc, r.left, r.top, r.right, r.bottom, hdcMem, r.left, r.top, SRCCOPY);
	}

	DeleteDC(hdcMem);
	DeleteObject(bm);
}



void CCGWorkView::OnDraw(CDC* pDC) {
	OnPaint();
}



/////////////////////////////////////////////////////////////////////////////
// CCGWorkView CGWork Finishing and clearing...

void CCGWorkView::OnDestroy() 
{
	CView::OnDestroy();

	// delete the DC
	if ( m_pDC ) {
		delete m_pDC;
	}

	if (m_pDbDC) {
		delete m_pDbDC;
	}
}



/////////////////////////////////////////////////////////////////////////////
// User Defined Functions

void CCGWorkView::RenderScene() {
	// do nothing. This is supposed to be overriden...

	return;
}


void CCGWorkView::OnFileLoad() 
{
	TCHAR szFilters[] = _T ("IRIT Data Files (*.itd)|*.itd|All Files (*.*)|*.*||");

	CFileDialog dlg(TRUE, _T("itd"), _T("*.itd"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY ,szFilters);

	if (dlg.DoModal () == IDOK) {
		isModelLoaded = false;

		// Create Model
		Scene::GetInstance().CreateModel();

		// Load model from file
		m_strItdFileName = dlg.GetPathName();		// Full path and filename
		PngWrapper p;
		CGSkelProcessIritDataFiles(m_strItdFileName, 1);

		// Get New Model and Camera
		Camera* camera = Scene::GetInstance().GetCamera();
		Model* model = Scene::GetInstance().GetSelectedModel();
		
		// Build Bounding Box
		model->BuildBoundingBox();
		// Get Bounding Box location and dimensions
		Vec4 bboxCenter = model->GetBBoxCenter();
		Vec4 bboxDim = model->GetBBoxDimensions();
		double maxDim = max(max(bboxDim[0], bboxDim[1]), bboxDim[2]);
		std::stringstream ss;
		ss << "Bounding Box Dimensions: " << bboxDim << ".";
		WriteToStatusBar(ss.str().c_str());

		// Set Camera position
		double radius = maxDim / 2.0;
		double f = sin(ToRadians(camera->GetPerspectiveParameters().FOV / 2.0));
		double offset = 3.0;
		if (bboxDim[0] > bboxDim[1])
		{
			f = sin(ToRadians(m_AspectRatio * camera->GetPerspectiveParameters().FOV / 2.0));
			offset = 3.5;
		}
		double zPos = abs(radius / f) * offset;
		if (zPos > abs(camera->GetCameraParameters().Eye[2]))
		{
			Vec4 eye = bboxCenter - Vec4(0.0, 0.0, zPos);
			camera->LookAt(eye, bboxCenter, Vec4(0.0, 1.0, 0.0));

			std::stringstream ss;
			ss << "Bounding Box Dimensions: " << bboxDim << ". Moved Camera to: " << eye;
			WriteToStatusBar(ss.str().c_str());

			// Set Perspective projection
#ifdef D_PERSP
			camera->SetPerspective2(45.0, m_AspectRatio, 0.1, 2.0);
#else
			camera->SetPerspective(45.0, m_AspectRatio, 1.0, 1000.0);
#endif

			// Set orthographic projection dimensions
			double orthoOffset = 1.0;
			orthoHeight = abs(maxDim) + orthoOffset;
			camera->SetOrthographic(orthoHeight, m_AspectRatio, 1.0, 1000.0);

			// Switch camera to the correct projection (the one that is selected)
			camera->SwitchProjection(m_bIsPerspective);
		}
		
		// Set ColorDialog model color
		m_colorDialog.WireframeColor = RGB(model->GetColor()[0], model->GetColor()[1], model->GetColor()[2]);

		// Model material using the Material dialog
		Material* material = model->GetMaterial();
		material->Ka = m_materialDialog.GetAmbientCoeffs();
		material->Kd = m_materialDialog.GetDiffuseCoeffs();
		material->Ks = m_materialDialog.GetShininessCoeffs();
		material->Specular = m_materialDialog.GetSpecularCoeff();

		// Clear Animation
		for (Model* model : Scene::GetInstance().GetModels())
			model->GetAnimation()->ClearAnimation();
		isRecording = false;

		isModelLoaded = true;
		Invalidate();	// force a WM_PAINT for drawing.
	} 
}

// VIEW HANDLERS ///////////////////////////////////////////

// Note: that all the following Message Handlers act in a similar way.
// Each control or command has two functions associated with it.

void CCGWorkView::OnViewOrthographic() 
{
	m_nView = ID_VIEW_ORTHOGRAPHIC;
	m_bIsPerspective = false;

	Scene::GetInstance().GetCamera()->SwitchProjection(m_bIsPerspective);

	Invalidate();		// redraw using the new view.
}

void CCGWorkView::OnUpdateViewOrthographic(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_nView == ID_VIEW_ORTHOGRAPHIC);
}

void CCGWorkView::OnViewPerspective() 
{
	m_nView = ID_VIEW_PERSPECTIVE;
	m_bIsPerspective = true;

	Scene::GetInstance().GetCamera()->SwitchProjection(m_bIsPerspective);

	Invalidate();
}

void CCGWorkView::OnUpdateViewPerspective(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_nView == ID_VIEW_PERSPECTIVE);
}




// ACTION HANDLERS ///////////////////////////////////////////

void CCGWorkView::OnActionRotate() 
{
	m_nAction = ID_ACTION_ROTATE;
}

void CCGWorkView::OnUpdateActionRotate(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_nAction == ID_ACTION_ROTATE);
}

void CCGWorkView::OnActionTranslate() 
{
	m_nAction = ID_ACTION_TRANSLATE;
}

void CCGWorkView::OnUpdateActionTranslate(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_nAction == ID_ACTION_TRANSLATE);
}

void CCGWorkView::OnActionScale() 
{
	m_nAction = ID_ACTION_SCALE;
}

void CCGWorkView::OnUpdateActionScale(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_nAction == ID_ACTION_SCALE);
}

void CCGWorkView::OnActionSelect()
{
	m_nAction = ID_ACTION_SELECT;
}


void CCGWorkView::OnUpdateActionSelect(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nAction == ID_ACTION_SELECT);
}




// AXIS HANDLERS ///////////////////////////////////////////


// Gets calles when the X button is pressed or when the Axis->X menu is selected.
// The only thing we do here is set the ChildView member variable m_nAxis to the 
// selected axis.
void CCGWorkView::OnAxisX() 
{
	m_isAxis_X = !m_isAxis_X;
}

// Gets called when windows has to repaint either the X button or the Axis pop up menu.
// The control is responsible for its redrawing.
// It sets itself disabled when the action is a Scale action.
// It sets itself Checked if the current axis is the X axis.
void CCGWorkView::OnUpdateAxisX(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_isAxis_X);
}


void CCGWorkView::OnAxisY() 
{
	m_isAxis_Y = !m_isAxis_Y;
}

void CCGWorkView::OnUpdateAxisY(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_isAxis_Y);
}


void CCGWorkView::OnAxisZ() 
{
	m_isAxis_Z = !m_isAxis_Z;
}

void CCGWorkView::OnUpdateAxisZ(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_isAxis_Z);
}


// LIGHT SHADING HANDLERS ///////////////////////////////////////////

void CCGWorkView::OnLightShadingFlat() 
{
	m_nLightShading = ID_LIGHT_SHADING_FLAT;
	Invalidate();
}

void CCGWorkView::OnUpdateLightShadingFlat(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_nLightShading == ID_LIGHT_SHADING_FLAT);
}


void CCGWorkView::OnLightShadingGouraud() 
{
	m_nLightShading = ID_LIGHT_SHADING_GOURAUD;
	Invalidate();
}

void CCGWorkView::OnUpdateLightShadingGouraud(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_nLightShading == ID_LIGHT_SHADING_GOURAUD);
}

void CCGWorkView::OnLightShadingPhong()
{
	m_nLightShading = ID_LIGHT_SHADING_PHONG;
	Invalidate();
}


void CCGWorkView::OnUpdateLightShadingPhong(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nLightShading == ID_LIGHT_SHADING_PHONG);
}

// LIGHT SETUP HANDLER ///////////////////////////////////////////

void CCGWorkView::OnLightConstants() 
{
	CLightDialog dlg;

	for (int id=LIGHT_ID_1;id<MAX_LIGHT;id++)
	{	    
	    dlg.SetDialogData((LightID)id,m_lights[id]);
	}
	dlg.SetDialogData(LIGHT_ID_AMBIENT,m_ambientLight);

	if (dlg.DoModal() == IDOK) 
	{
	    for (int id=LIGHT_ID_1;id<MAX_LIGHT;id++)
	    {
		m_lights[id] = dlg.GetDialogData((LightID)id);
	    }
	    m_ambientLight = dlg.GetDialogData(LIGHT_ID_AMBIENT);
	}	
	Invalidate();
}

void CCGWorkView::OnTimer(UINT_PTR nIDEvent)
{
	CView::OnTimer(nIDEvent);
	if (nIDEvent == 1 && m_colorDialog.IsDiscoMode)
		Invalidate();

	if (isPlaying)
	{
		// Set png filename if needed
		if (saveToFile)
		{
			std::stringstream ss;
			ss << currentFrame;
			std::string temp = animFileName + ss.str() + ".png";
			imgToSave.SetFileName(temp.c_str());

			if (!imgToSave.InitWritePng())
			{
				return;
			}
		}

		// Render current frame and step to the next one
		Invalidate();
		bool isModelPlaying = false;
		for (Model* model : Scene::GetInstance().GetModels())
		{
			Animation* anim = model->GetAnimation();
			isModelPlaying = anim->StepToNextFrame(isLinearInterpolation) || isModelPlaying;
		}
		currentFrame++;
		isPlaying = isModelPlaying;
		
		// If finished playing the animation, reset it
		// And if it was rendererd to files, reset that as well
		if (!isPlaying)
		{
			for (Model* model : Scene::GetInstance().GetModels())
				model->GetAnimation()->ResetAnimation();
			currentFrame = 0;

			if (saveToFile)
			{
				AfxMessageBox(L"Rendered animation successfully", MB_OK | MB_ICONINFORMATION);
				saveToFile = false;
			}
		}
	}
}

void CCGWorkView::OnLButtonDown(UINT nFlags, CPoint point)
{
	prevMousePos = point;

	if (m_nAction == ID_ACTION_SELECT && Scene::GetInstance().GetModels().size() > 0)
	{
		mouseClicked = true;
		mouseClickPos = point;
		selectedPolys.clear();
	}

	if (isRecording)
	{
		m_MouseDownTicks = clock();
		translationOffset = Vec4(0.0);
		rotationOffset = Vec4(0.0);
		scaleOffset = Vec4(1.0);
	}

	CView::OnLButtonDown(nFlags, point);

	Invalidate();
}

void CCGWorkView::OnMouseMove(UINT nFlags, CPoint point)
{
	double dx = point.x - prevMousePos.x;

	if ((nFlags & MK_LBUTTON) == MK_LBUTTON)
	{
		Model* model = Scene::GetInstance().GetSelectedModel();
		if (model == nullptr)
			return;

		if (m_nAction == ID_ACTION_TRANSLATE)
		{
			Vec4 trans;
			trans[0] = m_isAxis_X ? dx / m_sensitivity[0] : 0.0;
			trans[1] = m_isAxis_Y ? -dx / m_sensitivity[0] : 0.0;
			trans[2] = m_isAxis_Z ? dx / m_sensitivity[0] : 0.0;

			trans[0] = (m_nCoordSpace == ID_BUTTON_OBJECT) ? trans[0] : -trans[0];
			trans[2] = (m_nCoordSpace == ID_BUTTON_OBJECT) ? trans[2] : -trans[2];

			model->Translate(Mat4::Translate(trans), (m_nCoordSpace == ID_BUTTON_OBJECT));
			translationOffset += trans;
		}
		else if (m_nAction == ID_ACTION_ROTATE)
		{
			double angle = dx / m_sensitivity[1];
			Mat4 R;
			if (m_isAxis_X)
			{
				angle = (m_nCoordSpace == ID_BUTTON_OBJECT) ? angle : -angle;
				R = Mat4::RotateX(angle);
				rotationOffset[0] += angle;
			}
			if (m_isAxis_Y)
			{
				R = Mat4::RotateY(angle);
				rotationOffset[1] += angle;
			}
			if (m_isAxis_Z)
			{
				angle = (m_nCoordSpace == ID_BUTTON_OBJECT) ? -angle : angle;
				R = Mat4::RotateZ(angle);
				rotationOffset[2] += angle;
			}

			if (aroundEye && (m_nCoordSpace == ID_BUTTON_VIEW))
			{
				Vec4 eye = Scene::GetInstance().GetCamera()->GetCameraParameters().EyeCam;
				model->Scale(Mat4::Translate(eye), (m_nCoordSpace == ID_BUTTON_OBJECT));
				model->Rotate(R, (m_nCoordSpace == ID_BUTTON_OBJECT));
				model->Scale(Mat4::Translate(-eye), (m_nCoordSpace == ID_BUTTON_OBJECT));
			}
			else
				model->Rotate(R, (m_nCoordSpace == ID_BUTTON_OBJECT));
		}
		else if (m_nAction == ID_ACTION_SCALE)
		{
			double scale = -dx / m_sensitivity[2];
			double x_trans = 1.0 + (m_isAxis_X ? scale : 0.0);
			double y_trans = 1.0 + (m_isAxis_Y ? scale : 0.0);
			double z_trans = 1.0 + (m_isAxis_Z ? scale : 0.0);

			x_trans = (x_trans < 0.01) ? 0.01 : x_trans;
			y_trans = (y_trans < 0.01) ? 0.01 : y_trans;
			z_trans = (z_trans < 0.01) ? 0.01 : z_trans;

			scaleOffset *= Vec4(x_trans, y_trans, z_trans);
			
			if (aroundEye && (m_nCoordSpace == ID_BUTTON_VIEW))
			{
				Vec4 eye = Scene::GetInstance().GetCamera()->GetCameraParameters().EyeCam;
				model->Scale(Mat4::Translate(eye), (m_nCoordSpace == ID_BUTTON_OBJECT));
				model->Scale(Mat4::Scale(x_trans, y_trans, z_trans), (m_nCoordSpace == ID_BUTTON_OBJECT));
				model->Scale(Mat4::Translate(-eye), (m_nCoordSpace == ID_BUTTON_OBJECT));
			}
			else
				model->Scale(Mat4::Scale(x_trans, y_trans, z_trans), (m_nCoordSpace == ID_BUTTON_OBJECT));
		}
		Invalidate();
	}

	prevMousePos = point;

	CView::OnMouseMove(nFlags, point);
}

void CCGWorkView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (isRecording)
	{
		if (Scene::GetInstance().GetSelectedModel() == nullptr)
			return;

		Animation* anim = Scene::GetInstance().GetSelectedModel()->GetAnimation();
		clock_t ticksDiff = clock() - m_MouseDownTicks;
		double timeDiff = (double)ticksDiff / CLOCKS_PER_SEC;

		Frame* keyFrame = new Frame();
		keyFrame->ObjToWorld = Scene::GetInstance().GetSelectedModel()->GetObjToWorldTransform();
		keyFrame->ViewTransform = Scene::GetInstance().GetSelectedModel()->GetViewTransform();
		keyFrame->FrameNumber = anim->GetLastFrameNumber() + (int)(timeDiff * (double)framesPerSeconds);
		keyFrame->Action[0] = (m_nAction == ID_ACTION_TRANSLATE);
		keyFrame->Action[1] = (m_nAction == ID_ACTION_SCALE);
		keyFrame->Action[2] = (m_nAction == ID_ACTION_ROTATE);
		keyFrame->ObjectSpace = (m_nCoordSpace == ID_BUTTON_OBJECT);
		keyFrame->AroundEye = aroundEye;

		if (m_nAction == ID_ACTION_TRANSLATE)
		{
			keyFrame->Translation = translationOffset;
		}
		else if (m_nAction == ID_ACTION_ROTATE)
		{
			keyFrame->Rotation = rotationOffset;
		}
		else
		{
			keyFrame->Scale = scaleOffset;
		}
		keyFrame->OriginalFrame = keyFrame->FrameNumber;

		anim->AddKeyFrame(keyFrame);

		std::stringstream ss;
		ss << "Added new key frame at frame: " << keyFrame->FrameNumber;
		WriteToStatusBar(ss.str().c_str());
	}

	CView::OnLButtonUp(nFlags, point);
}


void CCGWorkView::OnButtonBbox()
{
	isBBoxOn = !isBBoxOn;
	if (Scene::GetInstance().GetModels().size() == 0)
		return;

	Scene::GetInstance().SetBBoxState(isBBoxOn);

	Invalidate();
}


void CCGWorkView::OnUpdateButtonBbox(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(isBBoxOn);
}


void CCGWorkView::OnButtonVertNorm()
{
	if (Scene::GetInstance().GetModels().size() == 0)
		return;

	Scene::GetInstance().SetNormals(!(Scene::GetInstance().AreVertexNormalsOn()), 
		Scene::GetInstance().ArePolyNormalsOn());
	Invalidate();
}


void CCGWorkView::OnUpdateButtonVertNorm(CCmdUI *pCmdUI)
{
	if (Scene::GetInstance().GetModels().size() == 0)
		return;
	
	pCmdUI->SetCheck(Scene::GetInstance().AreVertexNormalsOn());
}


void CCGWorkView::OnButtonPolyNorm()
{
	if (Scene::GetInstance().GetModels().size() == 0)
		return;

	Scene::GetInstance().SetNormals(Scene::GetInstance().AreVertexNormalsOn(),
		!(Scene::GetInstance().ArePolyNormalsOn()));
	Invalidate();
}


void CCGWorkView::OnUpdateButtonPolyNorm(CCmdUI *pCmdUI)
{
	if (Scene::GetInstance().GetModels().size() == 0)
		return;

	pCmdUI->SetCheck(Scene::GetInstance().ArePolyNormalsOn());
}



void CCGWorkView::OnNormalCalculated()
{
	Scene::GetInstance().SetCalcNormalState(true);

	CWnd* pParent = GetParent();
	CMenu* pMenu = pParent->GetMenu();
	if (pMenu != NULL)
	{
		pMenu->CheckMenuItem(ID_NORMAL_CALCULATED, MF_CHECKED | MF_BYCOMMAND);
		pMenu->CheckMenuItem(ID_NORMAL_FROMFILE, MF_UNCHECKED | MF_BYCOMMAND);
	}
	Invalidate();
}

void CCGWorkView::OnNormalFromfile()
{
	Scene::GetInstance().SetCalcNormalState(false);

	CWnd* pParent = GetParent();
	CMenu* pMenu = pParent->GetMenu();
	if (pMenu != NULL)
	{
		pMenu->CheckMenuItem(ID_NORMAL_CALCULATED, MF_UNCHECKED | MF_BYCOMMAND);
		pMenu->CheckMenuItem(ID_NORMAL_FROMFILE, MF_CHECKED | MF_BYCOMMAND);
	}
	Invalidate();
}

void CCGWorkView::OnButtonColors()
{
	isCColorDialogOpen = true;
	if (m_colorDialog.DoModal() == IDOK)
	{
		if (Scene::GetInstance().GetSelectedModel() != nullptr)
		{
			Model* model = Scene::GetInstance().GetSelectedModel();
			COLORREF oldModelColor = RGB((BYTE)model->GetColor()[0], (BYTE)model->GetColor()[1],
				(BYTE)model->GetColor()[2]);
			Vec4 normColor(GetRValue(m_colorDialog.NormalColor), GetGValue(m_colorDialog.NormalColor),
				GetBValue(m_colorDialog.NormalColor));

			if (oldModelColor != m_colorDialog.WireframeColor)
			{
				Vec4 modelColor(GetRValue(m_colorDialog.WireframeColor), GetGValue(m_colorDialog.WireframeColor),
					GetBValue(m_colorDialog.WireframeColor));
				model->SetColor(modelColor);
				showGeos = false;
			}
			model->SetNormalColor(normColor);

			isCColorDialogOpen = false;
		}

		Invalidate();
	}
}

void CCGWorkView::OnOptionsMousesensitivity()
{
	if (m_sensitivityDialog.DoModal() == IDOK)
	{
		m_sensitivity = m_sensitivityDialog.GetSensitivity();
		Invalidate();
	}
}


void CCGWorkView::OnOptionsTessellationtolerance()
{
	
	if (m_resolutionDialog.DoModal() == IDOK)
	{
		// Save fineness in Scene
		Scene::GetInstance().SetFineNess(m_resolutionDialog.Resolution);

		if (Scene::GetInstance().GetModels().size() > 0)
		{
			// Delete previous models
			Scene::GetInstance().GetSelectedModel()->DeleteGeometries();
			// Load model from file
			PngWrapper p;
			CGSkelProcessIritDataFiles(m_strItdFileName, 1);
			// Build Building Box
			Scene::GetInstance().GetSelectedModel()->BuildBoundingBox();

			Invalidate();
		}
	}
}


void CCGWorkView::OnButtonView()
{
	m_nCoordSpace = ID_BUTTON_VIEW;
}


void CCGWorkView::OnUpdateButtonView(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nCoordSpace == ID_BUTTON_VIEW);
}


void CCGWorkView::OnButtonObject()
{
	m_nCoordSpace = ID_BUTTON_OBJECT;
}


void CCGWorkView::OnUpdateButtonObject(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nCoordSpace == ID_BUTTON_OBJECT);
}


void CCGWorkView::OnOptionsPerspectivecontrol()
{
	Camera* camera = Scene::GetInstance().GetCamera();
	PerspectiveParams pParams = camera->GetPerspectiveParameters();
#ifdef D_PERSP
	m_perspDialog.ProjPlane = pParams.Far;
#else
	m_perspDialog.ProjPlane = pParams.Near;
#endif
	m_perspDialog.FOV = pParams.FOV;

	if (m_perspDialog.DoModal() == IDOK)
	{
#ifdef D_PERSP
		camera->SetPerspective2(m_perspDialog.FOV, pParams.AspectRatio, pParams.Near, m_perspDialog.ProjPlane);
#else
		camera->SetPerspective(pParams.Left, pParams.Right, pParams.Top, pParams.Bottom,
			m_perspDialog.ProjPlane, pParams.Far);
#endif
		camera->SwitchProjection(m_bIsPerspective);

		Invalidate();
	}
}


void CCGWorkView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (Scene::GetInstance().GetModels().size() > 0)
	{
		if (Scene::GetInstance().ArePolyNormalsOn() ||
			Scene::GetInstance().AreVertexNormalsOn())
		{
			if (nChar == VK_DOWN)
			{
				normalSizeFactor -= 0.01;
				if (normalSizeFactor < 0.01)
					normalSizeFactor = 0.01;
			}
			if (nChar == VK_UP)
			{
				normalSizeFactor += 0.01;
			}
		}
	}

	if (nChar == 0x47) // G key
		showGeos = !showGeos;
	if (nChar == 0x45) // E key
		aroundEye = !aroundEye;
	if (nChar == 0x44) // D key
		m_colorDialog.IsDiscoMode = !m_colorDialog.IsDiscoMode;
	if (nChar == 0x57) // W key
		currentPolySelection = WIREFRAME;
	if (nChar == 0x53) // S key
		currentPolySelection = SOLID_SCREEN;
	if (nChar == 0x31) // 1 key
		m_nLightShading = ID_LIGHT_SHADING_FLAT;
	if (nChar == 0x32) // 2 key
		m_nLightShading = ID_LIGHT_SHADING_GOURAUD;
	if (nChar == 0x33) // 3 key
		m_nLightShading = ID_LIGHT_SHADING_PHONG;
	if (nChar == 0x46) // F key
		fog.IsEnabled = !fog.IsEnabled;
	if (nChar == VK_OEM_PLUS) // + key  
	{
		OnPlaybackspeedIncrease();
	}
	if (nChar == VK_OEM_MINUS) // - Key 
	{
		OnPlaybackspeedDecrease();
	}

	if (nChar == 0x50) // P key
	{
		OnAnimationPlay();
	}
	if (nChar == 0x52) // R key
	{
		OnAnimationRecord();
	}
	if (nChar == 0x41) // A key
		m_antiAliasing.IsEnabled = !m_antiAliasing.IsEnabled;
	if (nChar == VK_LEFT)
		Scene::GetInstance().SelectPreviousModel();
	if (nChar == VK_RIGHT)
		Scene::GetInstance().SelectNextModel();
	if (nChar == VK_SPACE)
		Scene::GetInstance().UnselectAllModels();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
	Invalidate();
}


void CCGWorkView::OnRenderingWireframe()
{
	currentPolySelection = WIREFRAME;
	Invalidate();
}


void CCGWorkView::OnUpdateRenderingWireframe(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(currentPolySelection == WIREFRAME);
}


void CCGWorkView::OnRenderingSolidonscreen()
{
	currentPolySelection = SOLID_SCREEN;
	Invalidate();
}


void CCGWorkView::OnUpdateRenderingSolidonscreen(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(currentPolySelection == SOLID_SCREEN);
}


void CCGWorkView::OnRenderingSolidtofile()
{
	// Set dialog size variables to be the screen size
	if ((m_exportDialog.GetWidth() == 0) || (m_exportDialog.GetHeight() == 0))
	{
		m_exportDialog.SetWidth(m_WindowWidth);
		m_exportDialog.SetHeight(m_WindowHeight);
	}

	if (m_exportDialog.DoModal() == IDOK) {
		imgWidth = m_exportDialog.GetWidth();
		imgHeight = m_exportDialog.GetHeight();
		CT2A filename(m_exportDialog.GetFileName());

		imgToSave.SetWidth(imgWidth);
		imgToSave.SetHeight(imgHeight);
		imgToSave.SetFileName(filename);
		if (!imgToSave.InitWritePng()) return;

		saveToFile = true;
		Invalidate();
 	}
}


void CCGWorkView::OnBackgroundStretch()
{
	isBGStretch = true;
	Invalidate();
}


void CCGWorkView::OnUpdateBackgroundStretch(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(isBGStretch);
}


void CCGWorkView::OnBackgroundRepeat()
{
	isBGStretch = false;
	Invalidate();
}


void CCGWorkView::OnUpdateBackgroundRepeat(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(!isBGStretch);
}


void CCGWorkView::OnBackgroundOpen()
{
	TCHAR szFilters[] = _T("PNG Image Files (*.png)");

	CFileDialog dlg(TRUE, _T("png"), _T("*.png"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);

	if (dlg.DoModal() == IDOK) {
		BGFile = dlg.GetPathName();
		isBGFileOpen = true; 

		Invalidate();
	}
}


void CCGWorkView::OnLightSetmaterial()
{
	if (m_materialDialog.DoModal() == IDOK)
	{
		if (Scene::GetInstance().GetSelectedModel() != nullptr)
		{
			Material* material = Scene::GetInstance().GetSelectedModel()->GetMaterial();
			material->Ka = m_materialDialog.GetAmbientCoeffs();
			material->Kd = m_materialDialog.GetDiffuseCoeffs();
			material->Ks = m_materialDialog.GetShininessCoeffs();
			material->Specular = m_materialDialog.GetSpecularCoeff();
		}

		Invalidate();
	}
}


void CCGWorkView::OnButtonBculling()
{
	isBFCulling = !isBFCulling;
	Invalidate();
}


void CCGWorkView::OnButtonSil()
{
	showSil = !showSil;
	Invalidate();
}


void CCGWorkView::OnButtonInverseN()
{
	if (Scene::GetInstance().GetSelectedModel() == nullptr)
		return;

	Model* model = Scene::GetInstance().GetSelectedModel();
	model->NormalSign = -1 * model->NormalSign;
	Invalidate();
}


void CCGWorkView::OnOptionsSilhouetteoptions()
{
	m_silDialog.Thickness = silThickness;
	m_silDialog.Color = silColor;
	INT_PTR result = m_silDialog.DoModal();

	if (result == IDOK)
	{
		silThickness = m_silDialog.Thickness;
		silColor = m_silDialog.Color;
		Invalidate();
	}
}


void CCGWorkView::OnBackgroundClear()
{
	isBGFileOpen = false;
	Invalidate();
}


void CCGWorkView::OnFogeffectEnable()
{
	fog.IsEnabled = !fog.IsEnabled;
	Invalidate();
}


void CCGWorkView::OnUpdateFogeffectEnable(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(fog.IsEnabled);
}


void CCGWorkView::OnFogeffectOptions()
{
	CFogFialog dlg;
	dlg.SetDialogData(fog);

	if (dlg.DoModal() == IDOK)
	{
		fog = dlg.GetDialogData();
	}

	if (fog.IsEnabled)
		Invalidate();
}


void CCGWorkView::OnFogeffectPortal()
{
	fog.IsPortal = !fog.IsPortal;
	if (fog.IsEnabled)
		Invalidate();
}


void CCGWorkView::OnUpdateFogeffectPortal(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(fog.IsPortal);
}


void CCGWorkView::OnAnimationRecord()
{
	if (Scene::GetInstance().GetModels().size() == 0)
		return;

	isRecording = !isRecording;

	for (Model* model : Scene::GetInstance().GetModels())
		AddFirstFrame(model);
}


void CCGWorkView::OnUpdateAnimationRecord(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(isRecording);
}


void CCGWorkView::OnAnimationPlay()
{
	bool noAnimation = true;
	for (Model* model : Scene::GetInstance().GetModels())
		noAnimation = noAnimation && (model->GetAnimation()->GetLastFrameNumber() < 0);
	if (noAnimation) return;

	isPlaying = !isPlaying;
	Invalidate();
}


void CCGWorkView::OnAnimationSavetoimages()
{
	bool noAnimation = true;
	for (Model* model : Scene::GetInstance().GetModels())
		noAnimation = noAnimation && (model->GetAnimation()->GetLastFrameNumber() < 0);
	if (noAnimation) return;

	// Set dialog size variables to be the screen size
	if ((m_exportDialog.GetWidth() == 0) || (m_exportDialog.GetHeight() == 0))
	{
		m_exportDialog.SetWidth(m_WindowWidth);
		m_exportDialog.SetHeight(m_WindowHeight);
	}

	if (m_exportDialog.DoModal() == IDOK) {
		imgWidth = m_exportDialog.GetWidth();
		imgHeight = m_exportDialog.GetHeight();
		CT2A filename(m_exportDialog.GetFileName());

		imgToSave.SetWidth(imgWidth);
		imgToSave.SetHeight(imgHeight);

		animFileName = std::string(filename);
		animFileName = animFileName.substr(0, animFileName.size() - 4);
		isPlaying = true;
		saveToFile = true;
	}
}


void CCGWorkView::OnInterpolationLinear()
{
	isLinearInterpolation = true;
}


void CCGWorkView::OnUpdateInterpolationLinear(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(isLinearInterpolation);
}


void CCGWorkView::OnInterpolationBezier()
{
	isLinearInterpolation = false;
}


void CCGWorkView::OnUpdateInterpolationBezier(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(!isLinearInterpolation);
}


void CCGWorkView::OnPlaybackspeedNormal()
{
	for (Model* model : Scene::GetInstance().GetModels())
	{
		model->GetAnimation()->NormalPlaybackSpeed();
	}
}


void CCGWorkView::OnPlaybackspeedIncrease()
{
	for (Model* model : Scene::GetInstance().GetModels())
	{
		model->GetAnimation()->ResetAnimation();
		model->GetAnimation()->IncreasePlaybackSpeed(25.0);
	}
}


void CCGWorkView::OnPlaybackspeedDecrease()
{
	for (Model* model : Scene::GetInstance().GetModels())
	{
		model->GetAnimation()->ResetAnimation();
		model->GetAnimation()->DecreasePlaybackSpeed(25.0);
	}
}


void CCGWorkView::OnAntiOptions()
{
	if (m_antiAliasingDialog.DoModal() == IDOK)
	{
		m_antiAliasing.Type = m_antiAliasingDialog.antiAliasing.Type;
		m_antiAliasing.Size = m_antiAliasingDialog.antiAliasing.Size;
		Invalidate();
	}
}


void CCGWorkView::OnAntiEnabled()
{
	m_antiAliasing.IsEnabled = !m_antiAliasing.IsEnabled;
	Invalidate();
}


void CCGWorkView::OnUpdateAntiEnabled(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_antiAliasing.IsEnabled);
}


void CCGWorkView::OnFileClearall()
{
	Scene::GetInstance().ClearScene();
	Invalidate();
}
