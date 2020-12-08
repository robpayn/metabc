class CarbonateEq
{
   public:
      CarbonateEq(){};
      CarbonateEq(
         double tempC,
         double eConduct
      );
      double tempC;
      double tempK;
      double eConduct;
      double ionicStrength;
      double daviesParam;
      double daviesExponent;
      double activityCoeffH;
      double activityCoeffOH;
      double activityCoeffHCO3;
      double activityCoeffCO3;
      double kDissocH2CO3App;
      double kDissocHCO3App;
      double kDissocH2OApp;
      double kHenryCO2;
      double kHenryCO2fromTempFunc;
      void reset(
         double tempC,
         double eConduct
      );
      void optfCO2FromDICTotalAlk
      (
         double concDIC,
         double totalAlk,
         double tolerance,
         double min,
         double max,
         double out[]
      );
      double optpHFromDICTotalAlk
      (
         double concDIC,
         double totalAlk,
         double tolerance,
         double min,
         double max
      );
      double calcTotalAlkFromDICpH
      (
         double concDIC,
         double pH
      );
      double calcfCO2FromDICpH
      (
         double concDIC,
         double pH
      );
};
