//
// Created by carma on 2/3/23.
//

#ifndef CARMA_TIME_LIB_SIMULATIONPLUGIN_H
#define CARMA_TIME_LIB_SIMULATIONPLUGIN_H

namespace fwha_stol::lib::time::plugin {

    class SimulationPlugin {
    private:
        virtual SimulationPlugin() = 0;

    protected:
        CarmaClock get_clock();

    protected:
        CarmaClock _clock;
    };

}

#endif //CARMA_TIME_LIB_SIMULATIONPLUGIN_H
