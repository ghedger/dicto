/* Dicto
 * log.h
 *
 * Copyright (C) 2014 Gregory P. Hedger
 * greg@hedgersoftware.com
 * 30329 112th Pl. SE
 * Auburn, WA 98092
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Also add information on how to contact you by electronic and paper mail.
 *
 */

#pragma once

enum _LOG_LEVEL {
    LOG_NONE = 0,
    LOG_INFO,
    LOG_DEBUG
};

typedef _LOG_LEVEL LOG_LEVEL;

LOG_LEVEL getVerbosity();
void setVerbosity( LOG_LEVEL );


#define VERBOSE_LOG(lev,a) if(getVerbosity() >= lev) {cout << a;}
#define SET_VERBOSITY_LEVEL(a) setVerbosity(a)
#define GET_LOG_VERBOSITY() (getVerbosity())
