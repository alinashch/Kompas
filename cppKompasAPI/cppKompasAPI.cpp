// cppKompasAPI.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <atlbase.h>
#include <iostream>




#import "C:\\Program Files\\ASCON\\KOMPAS-3D v21 Home\\Bin\\kAPI5.tlb"
#import "C:\\Program Files\\ASCON\\KOMPAS-3D v21 Home\\Bin\\ksConstants3D.tlb"



using namespace Kompas6API5;
using namespace Kompas6Constants3D;


void create(Kompas6API5::KompasObjectPtr kompas) {
    Kompas6API5::ksDocument3DPtr doc;
    doc = kompas->Document3D();
    doc->Create(false, true);
    doc = kompas->ActiveDocument3D();

    kompas->Visible = true;

    ksPartPtr Part;
    Part = doc->GetPart(Kompas6Constants3D::pTop_Part);

    ksEntityPtr sketch = Part->NewEntity(o3d_sketch);
    ksSketchDefinitionPtr sketchDef = sketch->GetDefinition();
    sketchDef->SetPlane(Part->GetDefaultEntity(o3d_planeXOZ));
    sketch->Create();
    ksDocument2DPtr doc2D = sketchDef->BeginEdit();

    doc2D->ksArcBy3Points(-40, 60, -41, 60, -50, 50, 1);
    doc2D->ksArcBy3Points(40, -60, 41, -60, 50, -50, 1);
    doc2D->ksArcBy3Points(50, 50, 41, 60, 40, 60, 1);
    doc2D->ksArcBy3Points(-50, -50, -41, -60, -40, -60, 1);



    doc2D->ksLineSeg(-50, 50, -50, -50, 1);
    doc2D->ksLineSeg(50, -50, 50, 50, 1);
    doc2D->ksLineSeg(40, -60, -40, -60, 1);
    doc2D->ksLineSeg(-40, 60, 40, 60, 1);


    


    doc2D->ksCircle(-40, 50, 5, 1);
    doc2D->ksCircle(-40, -50, 5, 1);
    doc2D->ksCircle(40, 50, 5, 1);
    doc2D->ksCircle(40, -50, 5, 1);

    sketchDef->EndEdit();
    ksEntityPtr extr = Part->NewEntity(o3d_bossExtrusion);
    ksBossExtrusionDefinitionPtr extrDef = extr->GetDefinition();
    extrDef->SetSideParam(TRUE, etBlind, 10, 0, FALSE);
    extrDef->directionType* (Kompas6Constants3D::dtNormal);
    extrDef->SetSketch(sketch);
    extr->Create();


    ksEntityCollectionPtr flFaces = Part->EntityCollection(o3d_face);
    for (int i = 0; i < flFaces->GetCount(); i++)
    {
        ksEntityPtr face = flFaces->GetByIndex(i);
        ksFaceDefinitionPtr def = face->GetDefinition();
        if (def->GetOwnerEntity() == extr)
        {
            double h, r;
            def->GetCylinderParam(&h, &r);
            if (h < 10)
            {

                face->Putname("circle_stand");
                face->Update();
                break;
            }

        }
    }

    doc->SaveAs(L"C:\\Users\\Alina\\Desktop\\деталь.m3d");

}


int main()
{
    CoInitialize(NULL);
    Kompas6API5::KompasObjectPtr kompas;
    HRESULT hRes;
    hRes = kompas.GetActiveObject(L"Kompas.Application.5");
    if (FAILED(hRes)) {
        kompas.CreateInstance(L"Kompas.Application.5");
    }
    create(kompas);
    CoUninitialize();

}