#include "planet.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <utimage.h>

static const double AU = 14.9597870;//(/1000km)

//-----------------------------------------------------------------------------------
// デフォルトコンストラクタ
//-----------------------------------------------------------------------------------
CPlanet::CPlanet()
{
  m_radius = 0;
  m_slices = 0;
  m_stacks = 0;
  
  m_day = 0.0;
  
  m_orbital_period = 1.0;
  m_rotation_rate = 0.0;
  
  m_ascending_node = 0.0;
  m_orbit_tilt = 0.0;
  m_perihelion = 0.0;
  m_mean_distance = 0.0;
  m_eccentricity = 0.0;
  
  m_texture = 0;
}

//-----------------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------------
CPlanet::~CPlanet()
{
  if( m_texture > 0 ) {
    glDeleteTextures( 1, &m_texture );
  }
}

//-----------------------------------------------------------------------------------
// テクスチャのロード
//-----------------------------------------------------------------------------------
void CPlanet::LoadPlanetTexture( const char* filename )
{
  GLubyte	*buf =NULL, *image;
  GLsizei	orgWidth, orgHeight, orgDepth;
  GLsizei	imageWidth, imageHeight, imageDepth;
  GLenum	format;
  
  glGenTextures(1, &m_texture);
  if((buf = readJPEGimage((char*)filename,&orgWidth,&orgHeight,&orgDepth)) == NULL){
    fprintf(stderr,"Texture read Error!!\n");
    exit(0);
  }
  
  defineNewImageSize(&orgWidth, &orgHeight, &imageWidth, &imageHeight);
  
  imageDepth = orgDepth;
  if(imageDepth >= 4) format = GL_RGBA;
  else if(imageDepth == 3) format = GL_RGB;
  else format = GL_LUMINANCE;
  
  image = (GLubyte *)malloc(imageWidth*imageHeight*imageDepth
			    *sizeof(GLubyte));
  
  glPixelStorei(GL_UNPACK_ALIGNMENT,1);

  gluScaleImage(format, orgWidth, orgHeight, GL_UNSIGNED_BYTE, buf,
		imageWidth, imageHeight, GL_UNSIGNED_BYTE, image);
    
  glBindTexture(GL_TEXTURE_2D, m_texture);
  gluBuild2DMipmaps(GL_TEXTURE_2D, imageDepth, imageWidth, imageHeight,
		    format, GL_UNSIGNED_BYTE, image);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  
  free(image);

  glEnable(GL_TEXTURE_2D);
  
}

//-----------------------------------------------------------------------------------
// 球を描画
//-----------------------------------------------------------------------------------
void CPlanet::DrawPlanetSphere()
{
  int strip_order[4][2] = {{0, 0}, {1, 0}, {0, 1}, {1, 1}};
  
  glPushMatrix();
  {
    glRotatef(90.0, 0.0, 1.0, 0.0);
    
    glRotatef(m_ascending_node/M_PI*180.0 +90.0, 0.0, 0.0, 1.0);
    glRotatef(-m_orbit_tilt, 0.0, 1.0, 0.0);
    glRotatef((m_perihelion-m_ascending_node)/M_PI*180.0 -90.0, 0.0, 0.0, 1.0);
    glTranslatef(-m_eccentricity * m_mean_distance * AU, 0.0, 0.0);
    
    glTranslatef( m_mean_distance * AU *sin(m_day/m_orbital_period*2.0*M_PI),
		  m_mean_distance * AU *cos(m_day/m_orbital_period*2.0*M_PI)
		  * sqrt(1-m_eccentricity*m_eccentricity),
		  0.0 );
    
    //　自転
    glRotatef( m_day*m_rotation_rate, 0.0, 0.0, 1.0 );
    
    GLfloat pos[3];
    GLfloat nvec[3];
    GLfloat tex_x, tex_y, theta, phi;
    GLfloat slices_deg, stacks_deg, slices_div, stacks_div;
    
    slices_deg = 360.0 / m_slices; slices_div = 1.0 / m_slices;
    stacks_deg = 180.0 / m_stacks; stacks_div = 1.0 / m_stacks;
    
    for(int j=0; j<m_stacks; j++){
      for(int i=0; i<m_slices; i++){
	
	glBegin(GL_TRIANGLE_STRIP);
	
	for(int k=0; k<4; k++){
	  
	  theta = (i+strip_order[k][0])*slices_deg -180.0;
	  phi   = (j+strip_order[k][1])*stacks_deg -90.0;
	  if(phi == -90.0) phi   =  0.01*stacks_deg -90.0;
	  if(phi ==  90.0) phi   = -0.01*stacks_deg +90.0;
	  
	  // テクスチャ座標
	  tex_x = (i+strip_order[k][0])*slices_div;
	  tex_y = (j+strip_order[k][1])*stacks_div;
	  if(tex_y == 0.0) tex_y =  0.01*stacks_div;
	  if(tex_y == 1.0) tex_y = -0.01*stacks_div +1.0;
	  
	  // 法線ベクトル
	  {
	    float sinT, sinP, cosT, cosP;

	    if(fabs(theta)== 90) cosT=0; else cosT=cos(theta*M_PI/180.0);
	    if(fabs(phi)== 90) cosP=0; else cosP=cos(phi*M_PI/180.0);
	    if(fabs(theta)==180) sinT=0; else sinT=sin(theta*M_PI/180.0);
	    if(fabs(phi)==180) sinP=0; else sinP=sin(phi*M_PI/180.0);
	    
	    nvec[0] = cosT *cosP;
	    nvec[1] = sinT *cosP;
	    nvec[2] = sinP;
	    
	  }
	  
	  // 頂点座標
	  {
	    float sinT, sinP, cosT, cosP;

	    if(fabs(theta)== 90) cosT=0; else cosT=cos(theta*M_PI/180.0);
	    if(fabs(phi)== 90) cosP=0; else cosP=cos(phi*M_PI/180.0);
	    if(fabs(theta)==180) sinT=0; else sinT=sin(theta*M_PI/180.0);
	    if(fabs(phi)==180) sinP=0; else sinP=sin(phi*M_PI/180.0);

	    pos[0] = m_radius *cosT *cosP;
	    pos[1] = m_radius *sinT *cosP;
	    pos[2] = m_radius *sinP;
    
	  }
	  
	  glNormal3f(nvec[0], nvec[1], nvec[2]);
	  glTexCoord2f(tex_x, tex_y);
	  glVertex3f(pos[0], pos[1], pos[2]);
	}
	glEnd();
	
      }
    }
	  
  }
  glPopMatrix();

}

//-----------------------------------------------------------------------------------
// 軌道を描く
//-----------------------------------------------------------------------------------
void CPlanet::DrawOrbit()
{ 
  glDisable(GL_LIGHTING);
  glDisable(GL_TEXTURE_2D);
  glColor3f(0.4, 0.4, 0.4 );
  
  glPushMatrix();
  {
    glRotatef(90.0, 0.0, 1.0, 0.0);
    
    glRotatef(m_ascending_node/M_PI*180.0 +90.0, 0.0, 0.0, 1.0);
    glRotatef(-m_orbit_tilt, 0.0, 1.0, 0.0);
    glRotatef((m_perihelion-m_ascending_node)/M_PI*180.0 -90.0, 0.0, 0.0, 1.0);
    glTranslatef(-m_eccentricity * m_mean_distance * AU, 0.0, 0.0);
    
    glPushMatrix();
    {
      glBegin(GL_LINE_LOOP);
      for(int i=0; i<360; i++)
	glVertex3f(m_mean_distance * AU *sin(i*M_PI/180.),
		   m_mean_distance * AU *cos(i*M_PI/180.)
		   * sqrt(1-m_eccentricity*m_eccentricity),
		   0.0);
      glEnd();
    }
    glPopMatrix();
  }
  glPopMatrix();
  
}

