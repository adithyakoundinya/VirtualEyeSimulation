
#include<vtkStructuredPointsReader.h>
#include<vtkOutlineFilter.h>
#include<vtkPolyDataMapper.h>
#include<vtkActor.h>
#include<vtkArrowSource.h>
#include<vtkGlyph3D.h>
#include<vtkPointSource.h>
#include<vtkStreamLine.h>
#include<vtkRenderer.h>
#include<vtkRenderWindow.h>
#include<vtkRenderWindowInteractor.h>
#include<vtkPoints.h>
#include<vtkUnsignedCharArray.h>
#include<vtkVertexGlyphFilter.h>
#include <vtkPointData.h>
#include <string>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <windows.h>
#include<vtkProperty.h>
#include<vtkCylinderSource.h>
#include<vtkTextActor.h>
#include<vtkProperty.h>
#include <vtkTextProperty.h>
#include <vtkJPEGReader.h>
#include<vtkImageActor.h>
#include<vtkCamera.h>
#include <vtkImageData.h>

#define ROWS 1339
#define COLS 3

float dataArray[ROWS][COLS];


vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();

vtkSmartPointer<vtkTextActor> altitude = vtkSmartPointer<vtkTextActor>::New();
vtkSmartPointer<vtkTextActor> x_coordinate = vtkSmartPointer<vtkTextActor>::New();
vtkSmartPointer<vtkTextActor> y_coordinate = vtkSmartPointer<vtkTextActor>::New();
vtkSmartPointer<vtkTextActor> z_coordinate = vtkSmartPointer<vtkTextActor>::New();

vtkSmartPointer<vtkCylinderSource> cylinderSource = vtkSmartPointer<vtkCylinderSource>::New();
vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();


char buffer[1024];

  int greenCount=0;
  int redCount=0;
  float sumx=0,sumy=0,sumz=0;

void readCSVFile()
{
	 
    FILE *file = fopen("glider.csv", "r");
    //float value;
	#pragma warning(disable:4700)  
    for (int i = 0; i < ROWS; i++) 
	{
        for (int j = 0; j < COLS; j++)
		 {
            fscanf(file, "%f,", &dataArray[i][j]);
			//printf("%f \n", dataArray[i][j]);
        }
    }
	fclose(file);
	

   /* for (int i = 0; i < ROWS; i++) 
	{
	    for (int j = 0; j < 4; j++) 
		{
	        printf("%f ", dataArray[i][j]);
	    }
	
    	printf("\n");
	}*/
}

 
int main(int, char *[])
{
	 cylinderSource->SetRadius(10.0);
	 cylinderSource->SetResolution(100);
	 cylinderSource->SetCenter(0,0,0);
	 cylinderSource->SetHeight(10);

	 mapper->SetInputConnection(cylinderSource->GetOutputPort());
	 actor->SetMapper(mapper);
	 actor->GetProperty()->SetOpacity(1);
	 actor->GetProperty()->SetDiffuseColor(0,1,0);
	vtkSmartPointer<vtkImageData> imageData;
	readCSVFile();
	vtkSmartPointer<vtkJPEGReader> jpegReader = vtkSmartPointer<vtkJPEGReader>::New();
    
	jpegReader->SetFileName ( "glider_map.jpg" );
    jpegReader->Update();
    imageData = jpegReader->GetOutput();
	vtkSmartPointer<vtkImageActor> imageActor = vtkSmartPointer<vtkImageActor>::New();
	#if VTK_MAJOR_VERSION <= 5
		imageActor->SetInput(imageData);
	#else
		imageActor->SetInputData(imageData);
	#endif
	
  double origin[3];
  double spacing[3];
  int extent[6];
  imageData->GetOrigin( origin );
  imageData->GetSpacing( spacing );
  imageData->GetExtent( extent );
 
  vtkCamera* camera = renderer->GetActiveCamera();
  camera->ParallelProjectionOn();
  double xc = origin[0] + 0.5*(extent[0] + extent[1])*spacing[0];
  double yc = origin[1] + 0.5*(extent[2] + extent[3])*spacing[1];
  //double xd = (extent[1] - extent[0] + 1)*spacing[0];
  double yd = (extent[3] - extent[2] + 1)*spacing[1];
  double d = camera->GetDistance();
  camera->SetParallelScale(0.5*yd);
  camera->SetFocalPoint(xc,yc,0.0);
  camera->SetPosition(xc,yc,d);

  


	unsigned char red[3] = {255, 0, 0};
	unsigned char green[3] = {0, 255, 0};
	unsigned char blue[3] = {0, 0, 255};
	float current=0,previous=0;
	renderWindow->AddRenderer(renderer);
    renderWindowInteractor->SetRenderWindow(renderWindow);
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkPolyData> pointsPolydata = vtkSmartPointer<vtkPolyData>::New();
	vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
	vtkSmartPointer<vtkUnsignedCharArray> colors = vtkSmartPointer<vtkUnsignedCharArray>::New();
	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	altitude->SetPosition(700,670);
	altitude->GetTextProperty()->SetFontSize ( 20 );
	x_coordinate->SetPosition(700,600);
	x_coordinate->GetTextProperty()->SetFontSize ( 20 );
	x_coordinate->GetTextProperty()->SetColor(0,0,0);
	y_coordinate->SetPosition(700,500);
	y_coordinate->GetTextProperty()->SetFontSize ( 20 );
	y_coordinate->GetTextProperty()->SetColor(0.713,0.258,0.956);
	
	//altitude->SetInput("PUE1");
	//altitude->GetTextProperty()->SetColor ( 0.0, 0.0, 0.0 );

	
	colors->SetNumberOfComponents(3);
	colors->SetName ("Colors");
	renderWindow->SetSize(1024,720);
	renderer->SetBackground(1,1,1);
	for(int i=0;i<ROWS;i++)
		{
			current = dataArray[i][2];
			if (current == 998.53)
			{
				snprintf(buffer, sizeof buffer, "Highest Altitute : %.2f m", current);
				y_coordinate->SetInput(buffer);
			}
			points->InsertNextPoint (dataArray[i][0],dataArray[i][1],dataArray[i][2]);
			pointsPolydata->SetPoints(points);
			
		  vtkSmartPointer<vtkVertexGlyphFilter> vertexFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
			#if VTK_MAJOR_VERSION <= 5
			  vertexFilter->SetInputConnection(pointsPolydata->GetProducerPort());
			#else
			  vertexFilter->SetInputData(pointsPolydata);
			#endif
			  vertexFilter->Update();
 
		  polydata->ShallowCopy(vertexFilter->GetOutput());
 
		  // Setup colors
		  snprintf(buffer, sizeof buffer, "Altitude: %.2f meters", current);

		  altitude->SetInput(buffer);
		  snprintf(buffer, sizeof buffer, "Coordinate X: %.2f m\nCoordinate Z: %.2f m", dataArray[i][0],dataArray[i][1]);
		  x_coordinate->SetInput(buffer);
		  if(current>previous)
		  {
			colors->InsertNextTypedTuple(green);
			altitude->GetTextProperty()->SetColor(0,1,0);
		  }
		  else if(current==previous)
		  {
			  colors->InsertNextTypedTuple(blue);
			  altitude->GetTextProperty()->SetColor(0,0,1);
		  }
		  else
		  {
			colors->InsertNextTypedTuple(red);
			altitude->GetTextProperty()->SetColor(1,0,0);
		  }
 
		  polydata->GetPointData()->SetScalars(colors);

		  previous=dataArray[i][2];
 
		  // Visualization
			#if VTK_MAJOR_VERSION <= 5
			  mapper->SetInputConnection(polydata->GetProducerPort());
			#else
			  mapper->SetInputData(polydata);
			#endif
 
		  actor->SetMapper(mapper);
		  if (i==0 || i==1339)
		  {
			 actor->GetProperty()->SetPointSize(20);
		  }
		  else
		  actor->GetProperty()->SetPointSize(5);
 		  renderer->AddActor(actor);
		  renderer->AddActor(altitude);
		  renderer->AddActor(x_coordinate);
		  renderer->AddActor(y_coordinate);
		  renderer->AddActor(imageActor);
		  renderer->AddActor(actor);
		  renderer->ResetCamera();
		  renderWindow->Render();
		  
}//end of for
	
	//renderer->ResetCamera();
	//renderWindow->Render();
	//renderer->GetActiveCamera()->SetPosition(-47.5305, 319.315, 0);   
   //renderer->GetActiveCamera()->SetFocalPoint(78.9121, 89.8372, 95.1229);   
   //renderer->GetActiveCamera()->SetViewUp(0.0, 0.00490127,0);  
	//renderer->ResetCamera();
    //renderer->GetActiveCamera()->SetClippingRange(42.8255, 2141.28);
	
	renderWindowInteractor->Start();
 
  return EXIT_SUCCESS;
}