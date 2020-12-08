#include "carbonate.h"

struct Metab_Output {
   double* cFixation;
   double* cRespiration;
};

struct MetabDo_Output {
   double* dox;
   double* doProduction;
   double* doConsumption;
   double* doEquilibration;
};

class MetabDo {
   public:
      // Constructors/Destructors

      MetabDo(){};
      MetabDo
      (
         double dailyGPP,
         double ratioDoCFix,
         double dailyER,
         double ratioDoCResp,
         double k600,
         double initialDO,
         double* time,
         double* temp,
         double* par,
         double parTotal,
         double* airPressure,
         double stdAirPressure,
         int length
      );
      virtual ~MetabDo();

      // Attributes

      double dailyGPP;
      double ratioDoCFix;
      double dailyER;
      double ratioDoCResp;
      double k600;
      double initialDO;
      double* time;
      double* temp;
      double* par;
      double parTotal;
      double* airPressure;
      double stdAirPressure;
      int length;
      double* dt;
      double* satDo;
      double* kDo;
      double (*densityCalculator)(double tempC);
      double (*satDoCalculator)(
         double tempC,
         double densityWater,
         double relativePressure
      );
      double (*kSchmidtDoCalculator)(double tempC, double k600);
      Metab_Output output;
      MetabDo_Output outputDo;

      // Methods

      void initialize(
         double dailyGPP,
         double ratioDoCFix,
         double dailyER,
         double ratioDoCResp,
         double k600,
         double initialDO,
         double* time,
         double* temp,
         double* par,
         double parTotal,
         double* airPressure,
         double stdAirPressure,
         int length
      );
      virtual void run() = 0;
      void setDensityCalculator(double (*function)(double tempC));
      void setSatDoCalculator(
         double (*function)(
            double tempC,
            double densityWater,
            double relativePressure
         )
      );
      void setkSchmidtDoCalculator(
         double (*function)(
            double tempC,
            double k600
         )
      );
};

class MetabForwardEulerDo : virtual public MetabDo {
   public:
      using MetabDo::MetabDo;
      void run();
};

class MetabCrankNicolsonDo : virtual public MetabDo {
   public:
      using MetabDo::MetabDo;
      void run();
};

struct MetabDic_Output {
   double* pCO2;
   double* dic;
   double* dicProduction;
   double* dicConsumption;
   double* co2Equilibration;
   double* pH;
};

class MetabDoDic : virtual public MetabDo {
public:

   // Constructors/Destructors

   MetabDoDic(){};
   MetabDoDic
   (
      double dailyGPP,
      double ratioDoCFix,
      double dailyER,
      double ratioDoCResp,
      double k600,
      double initialDO,
      double* time,
      double* temp,
      double* par,
      double parTotal,
      double* airPressure,
      double stdAirPressure,
      int length,
      double ratioDicCfix,
      double ratioDicCresp,
      double initialDIC,
      double* pCO2air,
      double* alkalinity
   );
   virtual ~MetabDoDic();

   // Attributes

   double ratioDicCfix;
   double ratioDicCresp;
   double initialDIC;
   double* pCO2air;
   double* alkalinity;
   double* kCO2;
   double* kH;
   double (*kSchmidtCO2Calculator)(double tempC, double k600);
   MetabDic_Output outputDic;
   CarbonateEq carbonateEq;

   // Methods

   void initialize(
      double dailyGPP,
      double ratioDoCFix,
      double dailyER,
      double ratioDoCResp,
      double k600,
      double initialDO,
      double* time,
      double* temp,
      double* par,
      double parTotal,
      double* airPressure,
      double stdAirPressure,
      int length,
      double ratioDicCfix,
      double ratioDicCresp,
      double initialDIC,
      double* pCO2air,
      double* alkalinity
   );
   virtual void run() = 0;
   void setkSchmidtCO2Calculator(
      double (*function)(
         double tempC,
         double k600
      )
   );
};

class MetabForwardEulerDoDic :
   public MetabDoDic,
   public MetabForwardEulerDo
{
   public:
      using MetabDoDic::MetabDoDic;
      void run();
};
