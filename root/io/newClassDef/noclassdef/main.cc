#include "TFile.h"
#include "TClonesArray.h"
#include "RootCaloHit.h"
#include "RootData.h"
#include "TROOT.h"
#include "TClass.h"
#include "TStreamerInfo.h"

RootClassVersion(RootPCfix,3)


bool verifyVersion(const char* name, Short_t vers) {
   TClass * cl = gROOT->GetClass(name);
   if (cl->GetClassVersion() != vers ) {
      std::cerr << "The version of " << name << " is " 
                << cl->GetClassVersion() << " instead of " 
                << vers << std::endl;
      return false;
   }
   return true;
}


#ifdef SHARED
int maintest()
#else
int main()
#endif
{
    gDebug = 0;

    TFile f("mytest.root","RECREATE");
    RootData rsrd((char *)"RootCaloHit", (unsigned)10);
    new(rsrd.data[0]) RootCaloHit(1.1,1.2,2,"test01",3);
    RootCaloHit * ph = ((RootCaloHit*)rsrd.data[0]);

#if 1
    std::cerr << "Writing Hit" << std::endl;
    ph->Write("myhit");
    ph->myPrint();
    
    //std::cerr << "Automatic Print" << std::endl;
    //ph->Dump();
    std::cerr << std::endl;
    
    std::cerr << "Reading Hit" << std::endl;
    ph = (RootCaloHit*)f.Get("myhit");
    ph->myPrint();
    std::cerr << std::endl;
#endif

#if 1
    std::cerr << "Writing Data" << std::endl;
    rsrd.Write("mydata");
    ((RootCaloHit*)rsrd.data[0])->myPrint();
    std::cerr << std::endl;
    
    std::cerr << "Reading Data" << std::endl;
    RootData *p = (RootData*)f.Get("mydata");
    ((RootCaloHit*)p->data[0])->myPrint();
    std::cerr << std::endl;
#endif

    
    RootPCobject *myobjp = new RootPCobject(110);
    RootPCobject2*myobjp2 = new RootPCobject2(111);

    std::cerr << "Writing multiple inherited objects" << std::endl;
    myobjp->Print();
    myobjp->Write("obj1");
    myobjp2->Print();
    myobjp2->Write("obj2");

    std::cerr << "Reading multiple inherited objects with old methods" << std::endl;
    myobjp = (RootPCobject*)((void*)f.Get("obj1"));
    //std::cerr << typeid(*myobjp).name() << endl;
    //myobjp->Print();
    
    myobjp2 = (RootPCobject2*)f.Get("obj2");
    //std::cerr << typeid(*myobjp2).name() << endl;
    myobjp2->Print();

    std::cerr << "Reading multiple inherited objects with new methods" << std::endl;
    myobjp = dynamic_cast<RootPCobject *>(f.Get("obj1"));
    myobjp->Print();

    myobjp2 = dynamic_cast<RootPCobject2 *>(f.Get("obj2"));
    myobjp2->Print();

    std::cerr << "Reading multiple inherited objects with newer methods" << std::endl;
    //myobjp = (MyClass*)f.Get("obj1",RootPCobject::Class());

#ifdef INC_FAILURE
    std::cerr << "Check the base class offset in the case of private (and protected) inheritance" << std::endl;

    TClass *cl = gROOT->GetClass(typeid(RootPrivPCobject));
    cl->Print();
    std::cerr << "base class offset for RootPrivPCobject's TObject is: " << 
      cl->GetBaseClassOffset(TObject::Class()) << std::endl;

    cl = gROOT->GetClass(typeid(RootPrivPCobject2));
    cl->Print();
    std::cerr << "base class offset for RootPrivPCobject2's TObject is: " << 
      cl->GetBaseClassOffset(TObject::Class()) << std::endl;
    std::cerr << "base class offset for RootPrivPCobject2's RootPCellID is: " << 
      cl->GetBaseClassOffset(gROOT->GetClass(typeid(RootPCellID))) << std::endl;
#endif

    f.Close();
        
    // Verify the class version:
    bool result = true;
    result &= verifyVersion("RootPCvirt",1);
    //no header file version setting yet result &= verifyVersion("RootPCellID",2);
    result &= verifyVersion("RootPCfix",3);
    result &= verifyVersion("RootPCnodict",1);
    //no class template wide version setting yet result &= verifyVersion("RootPCtemp<int>",5);
    
#ifdef INC_FAILURE
    RootCaloHit::Class()->GetStreamerInfo()->ls();
#endif
    
    return !result;
}

ClassImp(RootCaloHit)
ClassImp(RootData)
ClassImp(RootPCellID)
