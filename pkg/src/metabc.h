#include "carbonate.h"

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
class MetabDo {
   public:

      // Constructors/Destructors

      //!  Default constructor for instantiating with no initialization
      MetabDo(){};

      //!  Constructor for instantiating and initializing with all parameters and driving data
      /*!
       *   \param dailyGPP
       *     Value for daily gross primary production (effective molarity of C per day)
       *   \param ratioDoCFix
       *     The ratio of DO produced per C fixed (positive)
       *   \param dailyER
       *     Value for the daily ecosystem respiration (effective molarity of C per day)
       *   \param ratioDoCResp
       *     The ratio of Do consumed per C respired (negative)
       *   \param k600
       *     The gas exchange rate at a Schmidt number of 600 (per day)
       *   \param initialDO
       *     The initial DO concentration (molarity)
       *   \param time
       *     An array of values with the times for calculations (days)
       *   \param temp
       *     Water temperatures corresponding to each time (deg C)
       *   \param parDist
       *     Fractions of GPP corresponding to each time step (unitless, last element not used)
       *   \param airPressure
       *     Air pressures corresponding to each time (same units as std air pressure)
       *   \param stdAirPressure
       *     Air pressure at standard conditions (establishes units of air pressure used)
       *   \param length
       *     Integer number of elements in the time array
       */
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
         double* parDist,
         double* airPressure,
         double stdAirPressure,
         int length
      );

      //!  The deconstructor.
      /*!
       *   Frees dynamic memory used for calculated driving variables and output
       */
      virtual ~MetabDo();

      // Attributes

      //! Value for daily gross primary production (molar concentration per time)
      double dailyGPP;
      //! The ratio of DO produced per C fixed (positive)
      double ratioDoCFix;
      //! Value for the daily ecosystem respiration (molar concentration per time)
      double dailyER;
      //! The ratio of Do consumed per C respired (negative)
      double ratioDoCResp;
      //! The gas exchange rate at a Schmidt number of 600
      double k600;
      //! The initial DO concentration (molar)
      double initialDO;
      //! An array of values with the times for calculations
      double* time;
      //! Water temperatures corresponding to each time
      double* temp;
      //! Fractions of GPP corresponding to each time step (last element not used)
      double* parDist;
      //! Air pressures corresponding to each time (same units as std air pressure)
      double* airPressure;
      //!  The air pressure at standard conditions (establishes units of pressure)
      double stdAirPressure;
      //! The number of elements in the time array
      int length;
      //! Array of the durations of the time steps (last element not used)
      double* dt;
      //! Array of saturated DO concentrations corresponding to time elements
      double* satDo;
      //! Array of the gas exchange rates for DO (per day)
      double* kDo;
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
      //! Output structure for DO related output
      MetabDo_Output outputDo;

      // Methods

      //!  Method to initialize attributes of the model
      /*!
       *   Initializes the attributes. See the parameterized constructor
       *   for a description of the arguments
       *   \sa MetabDo(double, double, double, double, double, double, double*, double*, double*, double*, double, int)
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
         double* parDist,
         double* airPressure,
         double stdAirPressure,
         int length
      );

      //!  Abstract definition of the run method
      /*!
       *   Inhereting classes must implement a run method to execute the model
       */
      virtual void run() = 0;

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

   //!  Default constructor for instantiating with no initialization
   MetabDoDic(){};

   //!  Constructor for instantiating and initializing with all parameters and driving data
   /*!
    *   See the constructor for MetabDo for description of DO related arguments.
    *   \sa MetabDo(double, double, double, double, double, double, double*, double*, double*, double*, double, int)
    *
    *   \param ratioDicCfix
    *     Ratio of the moles of DIC consumed per mole of organic carbon fixed (negative)
    *   \param ratioDicCresp
    *     Ratio of the moles of DIC produced per mole of organic carbon respired (positive)
    *   \param initialDIC
    *     Initial DIC concentration (micromolarity)
    *   \param pCO2air
    *     Array of partial pressures of CO2 in the air (microatmospheres) for each time element
    *   \param alkalinity
    *     Array of alkalinities (micromolarity) for each time element
    */
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
      double* parDist,
      double* airPressure,
      double stdAirPressure,
      int length,
      double ratioDicCfix,
      double ratioDicCresp,
      double initialDIC,
      double* pCO2air,
      double* alkalinity
   );

   //!  The deconstructor.
   /*!
    *   Frees dynamic memory used for calculated driving variables and output
    */
   virtual ~MetabDoDic();

   // Attributes

   //! \sa MetabDoDic(double, double, double, double, double, double, double*, double*, double*, double*, double, int, double, double, double, double*, double*)
   double ratioDicCfix;
   //! \sa MetabDoDic(double, double, double, double, double, double, double*, double*, double*, double*, double, int, double, double, double, double*, double*)
   double ratioDicCresp;
   //! \sa MetabDoDic(double, double, double, double, double, double, double*, double*, double*, double*, double, int, double, double, double, double*, double*)   double ratioDicCresp;
   double initialDIC;
   //! \sa MetabDoDic(double, double, double, double, double, double, double*, double*, double*, double*, double, int, double, double, double, double*, double*)   double ratioDicCresp;
   double* pCO2air;
   //! \sa MetabDoDic(double, double, double, double, double, double, double*, double*, double*, double*, double, int, double, double, double, double*, double*)   double* pCO2air;
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

   //!  Method to initialize attributes of the model
   /*!
    *   Initializes the attributes. See the parameterized constructor
    *   for a description of the arguments
    *   \sa MetabDoDic(double, double, double, double, double, double, double*, double*, double*, double*, double, int, double, double, double, double*, double*)
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
      double* parDist,
      double* airPressure,
      double stdAirPressure,
      int length,
      double ratioDicCfix,
      double ratioDicCresp,
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

template <typename T>
class MetabLagrangeDo
{
   public:
      MetabLagrangeDo(){};
      MetabLagrangeDo
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
         double* parDist,
         double* airPressure,
         double stdAirPressure,
         int numParcels,
         int timeSteps
      );
      ~MetabLagrangeDo();

      // Attributes

      //! Value for daily gross primary production (molar concentration per time)
      double dailyGPP;
      //! The ratio of DO produced per C fixed (positive)
      double ratioDoCFix;
      //! Value for the daily ecosystem respiration (molar concentration per time)
      double dailyER;
      //! The ratio of Do consumed per C respired (negative)
      double ratioDoCResp;
      //! The gas exchange rate at a Schmidt number of 600
      double k600;
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
      //! Fractions of GPP corresponding to each time step (last element not used)
      double* parDist;
      //! Air pressures corresponding to each time (same units as std air pressure)
      double* airPressure;
      //!  The air pressure at standard conditions (establishes units of pressure)
      double stdAirPressure;
      //! The number of water parcels in the upstream and downstream arrays
      int numParcels;
      //! The number of elements in the time vector for each parcel
      int lengthTimeVector;
      //! Output structure for fundamental output
      Metab_Output output;
      //! Output structure for DO related output
      MetabDo_Output outputDo;

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
         double* parDist,
         double* airPressure,
         double stdAirPressure,
         int numParcels,
         int timeSteps
      );
};
