#include "carbonate.h"
#include "utilities.h"

//!  The structure for fundamental model output
/*!
 *   This is a structure of arrays used to store the
 *   fundamental model ouput
 */
struct Metab_Output {
   /*! Moles of carbon per liter fixed over a time step */
   double* cFixation;
   /*! Moles of carbon per liter respired over a time step */
   double* cRespiration;
};

class Metab {
   public:
      Metab();
      virtual ~Metab();

      //! Daily gross primary production (molarity of DOC equivalent per day)
      double dailyGPP;
      //! Daily ecosystem respiration (molarity of DOC equivalent per day)
      double dailyER;
      //! The gas exchange rate at a Schmidt number of 600 (per day)
      double k600;
      //! Length of arrays for calculations
      int length;

      //! Object that will determine how GPP should be distributed based on PAR
      ParDistCalculator parDistCalculator;
      //! Pointer to function used to calculate water density from water temperature
      double (*densityCalculator)(double tempC);
      //! Pointer to function used to calculate DO saturation concentration
      double (*satDoCalculator)(
         double tempC,
         double densityWater,
         double relativePressure
      );
      //! Pointer to function used to calculate DO gas exchange rate from k600
      double (*kSchmidtDoCalculator)(double tempC, double k600);


      //! Output structure for fundamental output
      Metab_Output output;

      void initialize(
         double dailyGPP,
         double dailyER,
         double k600,
         int length
      );

      //!  Abstract definition of the run method
      /*!
       *   Inhereting classes must implement a run method to execute the model
       */
      virtual void run() = 0;

      //!  Define the PAR distribution calculator to use
      /*!
       *   \param function
       *     Pointer to a function that calculates PAR distribution from provided time
       *     intervals and a total PAR.
       */
      void setPARDistCalculator(ParDistCalculator calculator);

      //!  Define the density calculator to use
      /*!
       *   \param function
       *     Pointer to a function that calculates water density from a provided
       *     temperature in deg C.
       */
      void setDensityCalculator(double (*function)(double tempC));

      //!  Define the saturated DO calculator to use
      /*!
       *   \param function
       *     Pointer to a function that calculates saturate DO concentration
       *     from the provided water temperature (deg C), water density
       *     (kilograms per liter), and air pressure relative to standard
       *     air pressure at sea level (fraction)
       */
      void setSatDoCalculator(
         double (*function)(
            double tempC,
            double densityWater,
            double relativePressure
         )
      );

      //!  Define the Schmidt number calculator to use
      /*!
       *   \param function
       *     Pointer to a function that calculates the gas exchange rate (per day)
       *     for oxygen based on the provided water temperature (deg C)
       *     and the gas exchange rate (per day) at a Schmidt number of 600
       */
      void setkSchmidtDoCalculator(
         double (*function)(
            double tempC,
            double k600
         )
      );
};

//!  The structure for dissolved oxygen model output
/*!
 *   This is a structure of arrays used to store the model
 *   output regarding dissolved oxygen dynamics
 */
struct MetabDo_Output {
   /*! Dissolved oxygen (DO) concentration (molar) */
   double* dox;
   /*! DO produced over a time step */
   double* doProduction;
   /*! DO consumed over a time step */
   double* doConsumption;
   /*! DO exchanged with air over a time step (positive in, negative out) */
   double* doEquilibration;
};

//!  An abstraction for dissolved oxygen metabolism models
/*!
 *   Classes for models that simulate the effects of metabolism on dissolved
 *   oxygen ultimately inherit from this abstract class that
 *   establishes the basic constructor, destructor, and attributes
 *   needed to be maintained by the model.
 */
class MetabDo : virtual public Metab {
   public:
      // Constructors and destructors
      MetabDo();
      virtual ~MetabDo();

      // Attributes

      //! Ratio of moles O2 produced to moles DIC-C fixed (positive)
      double ratioDoCFix;
      //! Ratio of moles O2 consumed to moles DIC-C respired (negative)
      double ratioDoCResp;
      //! \sa initialize(double, double, double, double, double, double, double*, double*, double*, double, double*, double, int)
      double initialDO;
      //! \sa initialize(double, double, double, double, double, double, double*, double*, double*, double, double*, double, int)
      double* time;
      //! \sa initialize(double, double, double, double, double, double, double*, double*, double*, double, double*, double, int)
      double* temp;
      //! \sa initialize(double, double, double, double, double, double, double*, double*, double*, double, double*, double, int)
      double* par;
      //! \sa initialize(double, double, double, double, double, double, double*, double*, double*, double, double*, double, int)
      double parTotal;
      //! \sa initialize(double, double, double, double, double, double, double*, double*, double*, double, double*, double, int)
      double* airPressure;
      //! \sa initialize(double, double, double, double, double, double, double*, double*, double*, double, double*, double, int)
      double stdAirPressure;

      //! Average PAR over the time steps (last element will be zero)
      double* parAvg;
      //! Fractions of GPP corresponding to each time step (last element not used)
      double* parDist;
      //! Array of the durations of the time steps (last element not used)
      double* dt;
      //! Array of saturated DO concentrations corresponding to time elements
      double* satDo;
      //! Array of the gas exchange rates for DO (per day)
      double* kDo;

      //! Output structure for DO related output
      MetabDo_Output outputDo;

      // Methods

      //!  Method to initialize attributes of the model
      /*!
       *   Initializes the attributes. See the parameterized constructor
       *   for a description of the arguments
       *
       *   \param dailyGPP
       *     Daily gross primary production (molarity of DOC equivalent per day)
       *   \param ratioDoCFix
       *     The ratio of moles O2 produced per moles OC fixed (positive)
       *   \param dailyER
       *     Daily ecosystem respiration (molarity of DOC equivalent per day)
       *   \param ratioDoCResp
       *     The ratio of moles O2 consumed per moles OC respired (negative)
       *   \param k600
       *     The gas exchange rate at a Schmidt number of 600 (per day)
       *   \param initialDO
       *     The initial DO concentration (molarity)
       *   \param time
       *     The times for calculations (days)
       *   \param temp
       *     Water temperatures corresponding to each time (deg C)
       *   \param par
       *     PAR associated with each time (units consistent with total PAR)
       *   \param parTotal
       *     PAR integrated over the full analysis time (units consistent with PAR)
       *   \param airPressure
       *     Air pressures corresponding to each time (same units as std air pressure)
       *   \param stdAirPressure
       *     Air pressure at standard conditions (establishes units of air pressure used)
       *   \param length
       *     Number of elements in the time array
       */
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

      //!  Abstract definition of the run method
      /*!
       *   Inhereting classes must implement a run method to execute the model
       */
      virtual void run() = 0;
};

//! An implementation of MetabDo using a Forward Euler type solution
class MetabForwardEulerDo : virtual public MetabDo {
   public:
      // Inherit constructors and destructors from base class
      using MetabDo::MetabDo;

      //!  Implements the run function abstracted in MetabDo
      /*!
       *   Runs the metabolism model for DO based on Forward Euler type
       *   linear approximations.
       *   \sa MetabDo::run()
       */
      void run();
};

//! An implementation of MetabDo using a Crank Nicolson type solution
class MetabCrankNicolsonDo : virtual public MetabDo {
public:
   // Inherit constructors and destructors from base class
   using MetabDo::MetabDo;

   /*!
    *   Runs the metabolism model for DO based on Crank Nicolson type
    *   linear approximations.
    *   \sa MetabDo::run()
    */
   void run();
};

class MetabLagrangeDo : virtual public Metab {
   public:
      MetabLagrangeDo();
      virtual ~MetabLagrangeDo();

      // Attributes

      //! Ratio of moles O2 produced to moles DIC-C fixed (positive)
      double ratioDoCFix;
      //! Ratio of moles O2 consumed to moles DIC-C respired (negative)
      double ratioDoCResp;
      //! Array of upstream DO concentrations (molar)
      double* upstreamDO;
      //! An array of times water parcels pass upstream
      double* upstreamTime;
      //! An array of times water parcels pass downstream
      double* downstreamTime;
      //! At array of temperatures when the water parcels pass upstream
      double* upstreamTemp;
      //! At array of temperatures when the water parcels pass downstream
      double* downstreamTemp;
      //! PAR values when the water parcels pass upstream
      double* upstreamPAR;
      //! PAR values when the water parcels pass downstream
      double* downstreamPAR;
      //! Total PAR
      double parTotal;
      //! Air pressures corresponding to each time (same units as std air pressure)
      double* airPressure;
      //!  The air pressure at standard conditions (establishes units of pressure)
      double stdAirPressure;
      //! The number of water parcels in the upstream and downstream arrays
      int numParcels;
      //! The number of elements in the time vector for each parcel
      int lengthTimeVector;
      //! Array of the durations of the parcel travel times

      double* travelTimes;
      //! Average PAR for each parcel's travel time
      double* parAvg;
      //! Fractions of GPP corresponding to each parcel's travel time
      double* parDist;
      //! Array of saturated DO concentrations when the parcels are passing the upstream end
      double* upstreamSatDo;
      //! Array of saturated DO concentrations when the parcels are passing the downstream end
      double* downstreamSatDo;
      //! Array of the gas exchange rates for DO (per day) when the parcels are passing the upstream end
      double* upstreamkDo;
      //! Array of the gas exchange rates for DO (per day) when the parcels are passing the downstream end
      double* downstreamkDo;

      //! Output structure for DO related output
      MetabDo_Output outputDo;

      // Methods

      //!  Allocate memory and initialize attributes
      /*!
       *   \param dailyGPP
       *     Daily gross primary production (molarity of DOC equivalent per day)
       *   \param ratioDoCFix
       *     The ratio of moles O2 produced per moles OC fixed (positive)
       *   \param dailyER
       *     Daily ecosystem respiration (molarity of DOC equivalent per day)
       *   \param ratioDoCResp
       *     The ratio of moles O2 consumed per moles OC respired (negative)
       *   \param k600
       *     The gas exchange rate at a Schmidt number of 600 (per day)
       *   \param upstreamDO
       *     DO for a parcel passing the upstream end (molarity)
       *   \param upstreamTime
       *     Time a parcel is passing the upstream end (days)
       *   \param downstreamTime
       *     Time a parcel is passing the downstream end (days)
       *   \param upstreamTemp
       *     Temperature of a parcel when passing the upstream end (deg C)
       *   \param downstreamTemp
       *     Temperature of a parcel when passing the downstream end (deg C)
       *   \param upstreamPar
       *     PAR on a parcel when passing the upstream end (units consistent with total PAR)
       *   \param downstreamPar
       *     PAR on a parcel when passing the downstream end (units consistent with total PAR)
       *   \param parTotal
       *     PAR integrated over the full analysis time (units consistent with PAR).
       *     Will be calculated as average of upstream and downstream totals if provided value is less than zero.
       *   \param airPressure
       *     Air pressures corresponding to each time (same units as std air pressure)
       *   \param stdAirPressure
       *     Air pressure at standard conditions (establishes units of air pressure used)
       *   \param numParcels
       *     Number of parcels for Lagrangian calculations (length of upstream and downstream data)
       *   \param length
       *     Number of elements in the time array created for each parcel.
       *     Some solution schemes may ignore this argument.
       */
      void initialize
      (
         double dailyGPP,
         double ratioDoCFix,
         double dailyER,
         double ratioDoCResp,
         double k600,
         double* upstreamDO,
         double* upstreamTime,
         double* downstreamTime,
         double* upstreamTemp,
         double* downstreamTemp,
         double* upstreamPAR,
         double* downstreamPAR,
         double parTotal,
         double* airPressure,
         double stdAirPressure,
         int numParcels,
         int timeSteps
      );

      //!  Abstract definition of the run method
      /*!
       *   Inhereting classes must implement a run method to execute the model
       */
      virtual void run() = 0;
};

//! An implementation of MetabLagrangeDo based on Crank Nicolson approximations in one time step
class MetabLagrangeCNOneStepDo : virtual public MetabLagrangeDo {
   public:
      // Inherit constructors and destructors from the base class
      using MetabLagrangeDo::MetabLagrangeDo;

      /*!
       *   Runs the metabolism model for DO based on linear approximations
       *   used in the provided generic class
       *   \sa MetabLagrangeDo::run()
       */
      void run();
};

//! An implementation of MetabLagrangeDo using a generic type to specify MetabDO type
template <class T>
class MetabLagrangeGenericDo : virtual public MetabLagrangeDo {
   public:
      // Inherit constructors and destructors from the base class
      using MetabLagrangeDo::MetabLagrangeDo;

      //! Array of MetabDo objects used for each parcel simulation
      MetabDo* parcelModels;

      /*!
       *   Runs the metabolism model for DO based on linear approximations
       *   used in the provided generic class
       *   \sa MetabLagrangeDo::run()
       */
      void run();
};
// Include the definition of the details of implemetation for this template
// of a generic class
#include "MetabLagrangeGenericDo.hpp"

//!  The structure for DIC related model output
/*!
 *   This is a structure of arrays used to store the
 *   model ouput associated with dissolved inorganic carbon dynamics
 */
struct MetabDic_Output {
   double* pCO2;
   double* dic;
   double* dicProduction;
   double* dicConsumption;
   double* co2Equilibration;
   double* pH;
};

//!  An abstraction for dissolved oxygen and dissolved inorganic carbon metabolism models
/*!
 *   Classes for models that simulate the effects of metabolism on dissolved
 *   oxygen and dissolved inorganic carbon ultimately inherit from this abstract class that
 *   establishes the basic constructor, destructor, and attributes
 *   needed to be maintained by the model.
 */
class MetabDoDic : virtual public MetabDo {
   public:
      // Constructors/Destructors

      //!  Creates a new object of the class
      MetabDoDic();

      //!  Destroys an object of the class
      /*!
       *   Frees dynamic memory used
       */
      virtual ~MetabDoDic();

      // Attributes

      //! Ratio of the moles of DIC-C consumed per mole of organic carbon fixed (negative)
      double ratioDicCFix;
      double ratioDicCResp;
      double initialDIC;
      double* pCO2air;
      double* alkalinity;
      //! Array of gas exchange rates (per day) for carbon dioxide for each time element
      double* kCO2;
      //! Array of henry's constants for carbon dioxide corresponding to each time element
      double* kH;
      //! Pointer to the function to use to calculate the gas exchange rate for carbon dioxide from the k600
      double (*kSchmidtCO2Calculator)(double tempC, double k600);
      //! Structure for DIC related output
      MetabDic_Output outputDic;
      //! The object to use for carbonate equilibrium calculations
      CarbonateEq carbonateEq;

      // Methods

      //!  Allocate memory and initialize attributes
      /*!
       *   \param dailyGPP
       *     Daily gross primary production (molarity of DOC equivalent per day)
       *   \param ratioDoCFix
       *     The ratio of moles O2 produced per moles OC fixed (positive)
       *   \param dailyER
       *     Daily ecosystem respiration (molarity of DOC equivalent per day)
       *   \param ratioDoCResp
       *     The ratio of moles O2 consumed per moles OC respired (negative)
       *   \param k600
       *     The gas exchange rate at a Schmidt number of 600 (per day)
       *   \param initialDO
       *     The initial DO concentration (molarity)
       *   \param time
       *     The times for calculations (days)
       *   \param temp
       *     Water temperatures corresponding to each time (deg C)
       *   \param par
       *     PAR associated with each time (units consistent with total PAR)
       *   \param parTotal
       *     PAR integrated over the full analysis time (units consistent with PAR)
       *   \param airPressure
       *     Air pressures corresponding to each time (same units as std air pressure)
       *   \param stdAirPressure
       *     Air pressure at standard conditions (establishes units of air pressure used)
       *   \param length
       *     Number of elements in the time array
       *
       *   \param ratioDicCFix
       *     Ratio of the moles of DIC-C consumed per mole of organic carbon fixed (negative)
       *   \param ratioDicCResp
       *     Ratio of the moles of DIC-C produced per mole of organic carbon respired (positive)
       *   \param initialDIC
       *     Initial DIC concentration (micromolarity)
       *   \param pCO2air
       *     Array of partial pressures of CO2 in the air (microatmospheres) for each time element
       *   \param alkalinity
       *     Array of alkalinities (micromolarity) for each time element
       */
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
         double ratioDicCFix,
         double ratioDicCResp,
         double initialDIC,
         double* pCO2air,
         double* alkalinity
      );

      //!  Abstract definition of the run method
      /*!
       *   Inhereting classes must implement a run method to execute the model
       */
      virtual void run() = 0;

      //!  Define the Schmidt number calculator to use
      /*!
       *   \param function
       *     Pointer to a function that calculates the gas exchange rate (per day)
       *     for carbon dioxide based on the provided water temperature (deg C)
       *     and the gas exchange rate (per day) at a Schmidt number of 600
       */
      void setkSchmidtCO2Calculator(
         double (*function)(
            double tempC,
            double k600
         )
      );
};

//!  An implementation of MetabDoDic using a Forward Euler type solution
/*!
 *   This class inherits form MetabDoDic to implement a DoDic model,
 *   and also inherits the functions of a MetabDo implementation
 *   to perform the DO calculations.
 */
class MetabForwardEulerDoDic :
   public MetabDoDic,
   public MetabForwardEulerDo
{
   public:

      // Inherit constructors and destructors from base class
      using MetabDoDic::MetabDoDic;

      // Methods

      //!  Implements the run function abstracted in MetabDoDic
      /*!
       *   Runs the metabolism model for DO and DIC based on Forward Euler type
       *   linear approximations.
       *   \sa MetabDoDic::run()
       */
      void run();
};

//!  An implementation of MetabDoDic using a Forward Euler type solution
/*!
 *   This class inherits form MetabDoDic to implement a DoDic model,
 *   and also inherits the functions of a MetabDo implementation
 *   to perform the DO calculations.
 */
class MetabCrankNicolsonDoDic :
   public MetabDoDic,
   public MetabCrankNicolsonDo
{
   public:

      // Inherit constructors and destructors from base class
      using MetabDoDic::MetabDoDic;

      // Attributes

      //! Minimum DIC for optimization algorithm
      double minDIC = 0;
      //! Maximum DIC for optimziation algorithm
      double maxDIC = 1e6;
      //! Precision tolerance for optimization algorithm
      double tolerance = 1e-5;

      // Methods

      //!  Implements the run function abstracted in MetabDoDic
      /*!
       *   Runs the metabolism model for DO and DIC based on Forward Euler type
       *   linear approximations.
       *   \sa MetabDoDic::run()
       */
      void run();
};

class MetabLagrangeDoDic : virtual public MetabLagrangeDo {
   public:
      // Constructors/Destructors

      //!  Create a new object of the class
      MetabLagrangeDoDic();

      //!  Destroy the object
      /*!
       *   Frees dynamic memory used
       */
      virtual ~MetabLagrangeDoDic();

      // Attributes

      //! \sa MetabDoDic(double, double, double, double, double, double, double*, double*, double*, double*, double, int, double, double, double, double*, double*)
      double ratioDicCFix;
      //! \sa MetabDoDic(double, double, double, double, double, double, double*, double*, double*, double*, double, int, double, double, double, double*, double*)
      double ratioDicCResp;
      //! \sa MetabDoDic(double, double, double, double, double, double, double*, double*, double*, double*, double, int, double, double, double, double*, double*)   double ratioDicCresp;
      double* upstreamDIC;
      //! \sa MetabDoDic(double, double, double, double, double, double, double*, double*, double*, double*, double, int, double, double, double, double*, double*)   double ratioDicCresp;
      double* pCO2air;
      //! \sa MetabDoDic(double, double, double, double, double, double, double*, double*, double*, double*, double, int, double, double, double, double*, double*)   double* pCO2air;
      double* upstreamAlkalinity;
      //! \sa MetabDoDic(double, double, double, double, double, double, double*, double*, double*, double*, double, int, double, double, double, double*, double*)   double* pCO2air;
      double* downstreamAlkalinity;

      //! pCO2 as a parcel passes the upstream end
      double* upstreampCO2;
      //! pH as a parcel passes the upstream end
      double* upstreampH;
      //! CO2 saturation concentration as parcel passes upstream end
      double* upstreamSatCO2;
      //! CO2 saturation concentration as parcel passes downstream end
      double* downstreamSatCO2;
      //! Gas exchange rate for CO2 as a parcel passes upstream end
      double* upstreamkCO2;
      //! Gas exchange rate for CO2 as a parcel passes downstream end
      double* downstreamkCO2;
      //! Henry's constant as a parcel passes the upstream end
      double* upstreamkH;
      //! Henry's constant as a parcel passes the downstream end
      double* downstreamkH;

      //! Pointer to the function to use to calculate the gas exchange rate for carbon dioxide from the k600
      double (*kSchmidtCO2Calculator)(double tempC, double k600);
      //! The object to use for carbonate equilibrium calculations
      CarbonateEq carbonateEq;

      //! Structure for DIC related output
      MetabDic_Output outputDic;

      // Methods

      //!  Allocate memory and initialize attributes
      /*!
       *   \param dailyGPP
       *     Daily gross primary production (molarity of DOC equivalent per day)
       *   \param ratioDoCFix
       *     The ratio of moles O2 produced per moles OC fixed (positive)
       *   \param dailyER
       *     Daily ecosystem respiration (molarity of DOC equivalent per day)
       *   \param ratioDoCResp
       *     The ratio of moles O2 consumed per moles OC respired (negative)
       *   \param k600
       *     The gas exchange rate at a Schmidt number of 600 (per day)
       *   \param upstreamDO
       *     DO for a parcel passing the upstream end (molarity)
       *   \param upstreamTime
       *     Time a parcel is passing the upstream end (days)
       *   \param downstreamTime
       *     Time a parcel is passing the downstream end (days)
       *   \param upstreamTemp
       *     Temperature of a parcel when passing the upstream end (deg C)
       *   \param downstreamTemp
       *     Temperature of a parcel when passing the downstream end (deg C)
       *   \param upstreamPar
       *     PAR on a parcel when passing the upstream end (units consistent with total PAR)
       *   \param downstreamPar
       *     PAR on a parcel when passing the downstream end (units consistent with total PAR)
       *   \param parTotal
       *     PAR integrated over the full analysis time (units consistent with PAR).
       *     Will be calculated as average of upstream and downstream totals if provided value is less than zero.
       *   \param airPressure
       *     Air pressures corresponding to each time (same units as std air pressure)
       *   \param stdAirPressure
       *     Air pressure at standard conditions (establishes units of air pressure used)
       *   \param numParcels
       *     Number of parcels for Lagrangian calculations (length of upstream and downstream data)
       *   \param length
       *     Number of elements in the time array created for each parcel.
       *     Some solution schemes may ignore this argument.
       *
       *   \param ratioDicCFix
       *     Ratio of the moles of DIC consumed per mole of organic carbon fixed (negative)
       *   \param ratioDicCResp
       *     Ratio of the moles of DIC produced per mole of organic carbon respired (positive)
       *   \param upstreamDIC
       *     DIC in parcels passing the upstream end (micromolarity)
       *   \param pCO2air
       *     Average of pCO2 in the air for each parcel
       *   \param upstreamAlkalinity
       *     Alkalinity in parcels passing the upstream end (micromolarity)
       *   \param downstreamAlkalinity
       *     Alkalinity in parcels passing the downstream end (micromolarity)
       */
      void initialize(
         double dailyGPP,
         double ratioDoCFix,
         double dailyER,
         double ratioDoCResp,
         double k600,
         double* upstreamDO,
         double* upstreamTime,
         double* downstreamTime,
         double* upstreamTemp,
         double* downstreamTemp,
         double* upstreamPAR,
         double* downstreamPAR,
         double parTotal,
         double* airPressure,
         double stdAirPressure,
         int numParcels,
         int timeSteps,
         double ratioDicCFix,
         double ratioDicCResp,
         double* upstreamDIC,
         double* pCO2air,
         double* upstreamAlkalinity,
         double* downstreamAlkalinity
      );

      //!  Abstract definition of the run method
      /*!
       *   Inhereting classes must implement a run method to execute the model
       */
      virtual void run() = 0;

      //!  Define the Schmidt number calculator to use
      /*!
       *   \param function
       *     Pointer to a function that calculates the gas exchange rate (per day)
       *     for carbon dioxide based on the provided water temperature (deg C)
       *     and the gas exchange rate (per day) at a Schmidt number of 600
       */
      void setkSchmidtCO2Calculator(
         double (*function)(
            double tempC,
            double k600
         )
      );
};

//! An implementation of MetabLagrangeDo based on Crank Nicolson approximations in one time step
class MetabLagrangeCNOneStepDoDic :
   public MetabLagrangeDoDic,
   public MetabLagrangeCNOneStepDo
{
   public:
      // Inherit constructors and destructors from the base class
      using MetabLagrangeDoDic::MetabLagrangeDoDic;

      //! Minimum DIC for optimization algorithm
      double minDIC = 0;
      //! Maximum DIC for optimziation algorithm
      double maxDIC = 1e6;
      //! Precision tolerance for optimization algorithm
      double tolerance = 1e-5;

      /*!
       *   Runs the metabolism model for DO based on linear approximations
       *   used in the provided generic class
       *   \sa MetabLagrangeDo::run()
       */
      void run();
};
