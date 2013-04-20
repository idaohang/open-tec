#pragma ident "$Id: RinexClockBase.hpp 3140 2012-06-18 15:03:02Z susancummins $"

/**
 * @file RinexClockBase.hpp
 * Base class for RinexClock file data
 */

#ifndef GPSTK_RINEX_CLOCK_BASE_INCLUDE
#define GPSTK_RINEX_CLOCK_BASE_INCLUDE

#include "FFData.hpp"

namespace gpstk
{
   /** @defgroup RinexClock RINEX Clock format file I/O */
   //@{

   /// This class is here to make readable inheritance diagrams.
   class RinexClockBase : public FFData
   {
   public:
         /// Destructor per the coding standards
      virtual ~RinexClockBase() {}
   };

   //@}

}  // namespace

#endif   // GPSTK_RINEX_CLOCK_BASE_INCLUDE
