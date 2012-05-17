/*
 * Copyright (C) DOLPHIN Project-Team, INRIA Futurs, 2006-2007
 * (C) OPAC Team, LIFL, 2002-2007
 *
 * (c) Antonio LaTorre <atorre@fi.upm.es>, 2007
 *
 * ===================================================================
 * Multi-Objective Support added by:
 * ===================================================================
 * Uninversity of Nottingham (UK), ASAP Research Group
 * (c) Juan Castro-Gutierrez <jpcastrog@gmail.com>, 2010
 * (c) Dario Landa-Silva <dario.landasilva@nottingham.ac.uk>, 2010
 *
 * Universidad de La Laguna (Spain), DEIOC
 * (c) José A. Moreno Pérez <jamoreno@ull.es>, 2010
 *
 * -------------------------------------------------------------------
 *
 * This software is governed by the CeCILL license under French law and
 * abiding by the rules of distribution of free software.  You can  use,
 * modify and/ or redistribute the software under the terms of the CeCILL
 * license as circulated by CEA, CNRS and INRIA at the following URL
 * "http://www.cecill.info".
 *
 * As a counterpart to the access to the source code and  rights to copy,
 * modify and redistribute granted by the license, users are provided only
 * with a limited warranty  and the software's author,  the holder of the
 * economic rights,  and the successive licensors  have only  limited liability.
 *
 * In this respect, the user's attention is drawn to the risks associated
 * with loading,  using,  modifying and/or developing or reproducing the
 * software by the user in light of its specific status of free software,
 * that may mean  that it is complicated to manipulate,  and  that  also
 * therefore means  that it is reserved for developers  and  experienced
 * professionals having in-depth computer knowledge. Users are therefore
 * encouraged to load and test the software's suitability as regards their
 * requirements in conditions enabling the security of their systems and/or
 * data to be ensured and,  more generally, to use and operate it in the
 * same conditions as regards security.
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL license and that you accept its terms.
 *
 * ParadisEO WebSite : http://paradiseo.gforge.inria.fr
 * Contact: paradiseo-help@lists.gforge.inria.fr
 *
 */

#ifndef _moeoVRPStat_h
#define _moeoVRPStat_h

// The base definition of eoInit
#include <utils/eoStat.h>

/**
  * \class moeoVRPStat moeoVRPStat.h
  * \brief Manages the statistics of the VRP problem.
  */

class moeoVRPStat : public eoStat<moeoVRP, double> {

public:

    /**
      * \brief Constructor: initializes variables properly.
      * \param _description A string identifying the class.
      */

    moeoVRPStat (std::string _description = "moeoVRPStat ") : eoStat<moeoVRP, double> (0.0, _description) {

    }


    /**
      * \brief Gets statistics from a population.
      * \param _pop The population that will be analyzed.
      */

    void operator() (const eoPop<moeoVRP>& _pop) {

        double tmpStat (0.);
        eoStat<moeoVRP, double>::value () = tmpStat;

    }


    /**
      * \brief Returns a string containing the name of the class. Used to display statistics.
      * \return The string containing the name of the class.
      */

    virtual std::string className (void) const {

        return "moeoVRPStat";

    }


private:

};

#endif
