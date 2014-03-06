#include "sgl.h"

using namespace std;
// --- Do not modify this code ---+
#define IMPLEMENT_THIS_FUNCTION printf("Warning: call to unimplimented function!\n")
STImage* img;
int buffer_width;
int buffer_height;
vector<STPoint2> vertices;
vector<STVector3> current_matrix;//each STVector3 corresponds to one column; for manipulating 2D points, the transform matrix is 3x3
vector<vector<STVector3> > matrix_stack;
STVector3 current_color;
vector<STVector3> colors;
void setBuffer(STImage* ptr) { img = ptr; }
void setBufferSize(int w, int h) { buffer_width = w; buffer_height = h; }
// --- End of do not modify this code ---+

void renderTriangles()
{
    if(vertices.size()<3){
        cout<<"To draw a triangle strip, you need to specify at least three vertices"<<endl;
        exit(0);
    }
    for(int v=0;v+2<vertices.size();v++){
        vector<STPoint2> triangle;
        vector<STVector3> triangle_color;
        STPoint2 vertex0=transform_by_current_matrix(vertices[v]);
        //cout<<"before transform "<<vertices[v].x<<" "<<vertices[v].y<<" after tranform "<<vertex0.x<<" "<<vertex0.y<<endl;
        STPoint2 vertex1=transform_by_current_matrix(vertices[v+1]);
        STPoint2 vertex2=transform_by_current_matrix(vertices[v+2]);
        if(counter_clockwise(vertex0,vertex1,vertex2)){
          triangle.push_back(vertex0);
          triangle.push_back(vertex1);
          triangle.push_back(vertex2);
          triangle_color.push_back(colors[v]);
          triangle_color.push_back(colors[v+1]);
          triangle_color.push_back(colors[v+2]);}
        else{
          triangle.push_back(vertex2);
          triangle.push_back(vertex1);
          triangle.push_back(vertex0);
          triangle_color.push_back(colors[v+2]);
          triangle_color.push_back(colors[v+1]);
          triangle_color.push_back(colors[v]);
        }
        rasterize(triangle,triangle_color);
    }
}

void sglBeginTriangles()
{
    vertices.clear();
    colors.clear();
}

void sglEnd()
{
    renderTriangles();
    vertices.clear();
    colors.clear();
}

void sglLoadIdentity()
{
    current_matrix.clear();
    current_matrix.push_back(STVector3::eX);
    current_matrix.push_back(STVector3::eY);
    current_matrix.push_back(STVector3::eZ);
}

void sglScale(SGLfloat xscale, SGLfloat yscale)
{
  current_matrix[0]*=xscale;
  current_matrix[1]*=yscale;
}

void sglTranslate(SGLfloat xtrans, SGLfloat ytrans)
{
  current_matrix[2]+=xtrans*current_matrix[0]+ytrans*current_matrix[1];
}

void sglRotate(SGLfloat angle)
{
  vector<STVector3> current_matrix_save=current_matrix;
  current_matrix[0]=current_matrix_save[0]*cos(angle)+current_matrix_save[1]*sin(angle);
  current_matrix[1]=current_matrix_save[1]*cos(angle)-current_matrix_save[0]*sin(angle);
}

void sglPushMatrix()
{
  matrix_stack.push_back(current_matrix);
}

void sglPopMatrix()
{
  current_matrix=matrix_stack.back();
  matrix_stack.pop_back();
}

void sglVertex(SGLfloat x, SGLfloat y)
{
    vertices.push_back(STPoint2(x,y));
    colors.push_back(current_color);
}

void sglColor(SGLfloat r, SGLfloat g, SGLfloat b)
{
    current_color=STVector3(r,g,b);
}

void fragment(int x,int y,const vector<STPoint2>& triangle,const vector<STVector3> triangle_color)
{
    STPoint2 p((float)x,(float)y);
    STVector3 weights=interpolation_weights(p,triangle);
    STVector3 color=triangle_color[0]*weights.x+triangle_color[1]*weights.y+triangle_color[2]*weights.z;

    STImage::Pixel pixel_color((int)(color.x*(float)255),(int)(color.y*(float)255),(int)(color.z*(float)255));

    img->SetPixel(x,y,pixel_color);
}

void makeline(const STPoint2& v0,const STPoint2& v1,line& l)
{
    l.a=v1.y-v0.y;
    l.b=v0.x-v1.x;
    l.c=-(l.a*v0.x+l.b*v0.y);
}

bool shadow(line& l)
{
    return (l.a>0) || (l.a==0 && l.b>0);
}

bool inside(int x,int y,line&l)
{
    float e=l.a*(float)x+l.b*(float)y+l.c;
    return (e==0)? !shadow(l):(e<0);
}

void bound(const vector<STPoint2>& triangle,bbox& b)
{
    b.xmin=ceil(min(min(triangle[0].x,triangle[1].x),triangle[2].x));
    b.xmax=ceil(max(max(triangle[0].x,triangle[1].x),triangle[2].x));
    b.ymin=ceil(min(min(triangle[0].y,triangle[1].y),triangle[2].y));
    b.ymax=ceil(max(max(triangle[0].y,triangle[1].y),triangle[2].y));
}

void rasterize(const vector<STPoint2>& triangle,const vector<STVector3> triangle_color)
{
    bbox b;bound(triangle,b);
    line l0,l1,l2;
    makeline(triangle[0],triangle[1],l2);
    makeline(triangle[1],triangle[2],l0);
    makeline(triangle[2],triangle[0],l1);
    for(int y=max(b.ymin,0);y<min(b.ymax,img->GetHeight());y++)
        for(int x=max(b.xmin,0);x<min(b.xmax,img->GetWidth());x++){
            if(inside(x,y,l0) && inside(x,y,l1) && inside(x,y,l2))
                fragment(x,y,triangle,triangle_color);
        }
}

float area(STPoint2 p1,STPoint2 p2,STPoint2 p3)
{
    STVector2 p1p2(p2.x-p1.x,p2.y-p1.y);
    STVector2 p1p3(p3.x-p1.x,p3.y-p1.y);

    return abs(p1p2.x*p1p3.y-p1p2.y*p1p3.x)/(float)2;
}

STVector3 interpolation_weights(STPoint2 p,const vector<STPoint2>& triangle)
{
    float tri_area=area(triangle[0],triangle[1],triangle[2]);
    float alpha0=area(p,triangle[1],triangle[2])/tri_area;
    float alpha1=area(p,triangle[0],triangle[2])/tri_area;
    float alpha2=area(p,triangle[0],triangle[1])/tri_area;
    return STVector3(alpha0,alpha1,alpha2);
}

bool counter_clockwise(STPoint2 p1,STPoint2 p2,STPoint2 p3)
{
    STVector2 p1p2(p2.x-p1.x,p2.y-p1.y);
    STVector2 p2p3(p3.x-p2.x,p3.y-p2.y);

    return p1p2.x*p2p3.y-p1p2.y*p2p3.x>(float)0;
}

STPoint2 transform_by_current_matrix(const STPoint2& p)
{
  STVector3 transformed_homogeneous_coordinate=current_matrix[0]*p.x+current_matrix[1]*p.y+current_matrix[2];
  STPoint2 transformed_point(transformed_homogeneous_coordinate.x,transformed_homogeneous_coordinate.y);
  return transformed_point;
}
