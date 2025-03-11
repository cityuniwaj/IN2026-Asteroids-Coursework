#include "GameUtil.h"
#include "Shape.h"

using namespace std;

Shape::Shape()
{
}

Shape::Shape(const string& shape_filename)
{
	LoadShape(shape_filename);
}

Shape::~Shape()
{
}

void Shape::Render(void)
{
	// Disable lighting for solid colour lines
	glDisable(GL_LIGHTING);
	// Start drawing lines
	if (mLoop) { glBegin(GL_LINE_LOOP); }
	else { glBegin(GL_LINE_STRIP); }
	// Set rgb colour
	glColor3f(mRGB[0], mRGB[1], mRGB[2]);
	// Add vertices to draw shape
	for (GLVector2fList::iterator it = mPoints.begin(); it != mPoints.end(); ++it) {
		shared_ptr<GLVector2f> vec = (*it);
		glVertex2f(vec->x, vec->y);
	}
	// Finish drawing lines
	glEnd();
	// Enable lighting
	glEnable(GL_LIGHTING);

}

void Shape::LoadShape(const string& shape_filename)
{
	string filename = "";
	filename += shape_filename;
	ifstream shape_file(filename.c_str(), ios::in | ios::binary);

	if (!shape_file) { cerr << "Error opening " << shape_filename; exit(1); }

	string s;
	shape_file >> s;
	if (0 == s.compare("loop")) { mLoop = true; }
	else { mLoop = false; }

	shape_file >> mRGB;
	while (!shape_file.eof()) {
		float x, y;
		shape_file >> x >> y;
		shared_ptr<GLVector2f> vec(new GLVector2f(x, y));
		mPoints.push_back(vec);
	}

}
