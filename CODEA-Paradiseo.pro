#-------------------------------------------------
#
# Project created by jpcastrog 5 Dec 2010
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = CODEA-Paradiseo
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

# We don't want to have this app in a bundle
CONFIG -= app_bundle
CONFIG += warn_off

# Only important warnings
QMAKE_CXXFLAGS += -W

# Paradiseo Local
# INCLUDEPATH += /opt/paradiseo-1.2.1/paradiseo-moeo/src \
#    /opt/paradiseo-1.2.1/paradiseo-moeo/src/utils \
#    /opt/paradiseo-1.2.1/paradiseo-eo/src \
#    /opt/paradiseo-1.2.1/paradiseo-eo/src/utils \
#    /opt/paradiseo-1.2.1/paradiseo-mo/src \
#    /opt/paradiseo-1.2.1/paradiseo-mo/src/utils \
#    core/

# LIBS += /opt/paradiseo-1.2.1/paradiseo-moeo/build/lib/libmoeo.a \
#         /opt/paradiseo-1.2.1/paradiseo-eo/build/lib/libeo.a \
#         /opt/paradiseo-1.2.1/paradiseo-eo/build/lib/libeoutils.a \
#         /home/jpc/.local/share/Trash/files/libs.4/boost/lib/libboost_thread-gcc43-mt-1_39.a

# Paradiseo Cluster
INCLUDEPATH += /work/work3/psxjpc/software/paradiseo-1.2.1/paradiseo-moeo/src \
   /work/work3/psxjpc/software/paradiseo-1.2.1/paradiseo-moeo/src/utils \
   /work/work3/psxjpc/software/paradiseo-1.2.1/paradiseo-eo/src \
   /work/work3/psxjpc/software/paradiseo-1.2.1/paradiseo-eo/src/utils \
   /work/work3/psxjpc/software/paradiseo-1.2.1/paradiseo-mo/src \
   /work/work3/psxjpc/software/paradiseo-1.2.1/paradiseo-mo/src/utils \
   /work/work3/psxjpc/software/boost_1_46_1/ \
   core/

LIBS += /work/work3/psxedc/software/paradiseo-1.2.1/paradiseo-moeo/build/lib/libmoeo.a \
        /work/work3/psxedc/software/paradiseo-1.2.1/paradiseo-eo/build/lib/libeo.a \
        /work/work3/psxedc/software/paradiseo-1.2.1/paradiseo-eo/build/lib/libeoutils.a \
        /work/work3/psxjpc/software/boost_1_46_1/stage/lib/libboost_thread.so



# Boost library
INCLUDEPATH += /opt/local/include/

SOURCES += main.cpp

HEADERS += \
    core/CODEATypes.h \
    core/neighborhood.h \
    libs/concurrentQueue.h \
    core/mailBox.h \
    core/agent.h \
    core/container.h \
    problems/VRPTW/moeoVRPUtils.h \
    problems/VRPTW/moeoVRPStat.h \
    problems/VRPTW/moeoVRPQuadCrossover.h \
    problems/VRPTW/moeoVRPObjectiveVectorTraits.h \
    problems/VRPTW/moeoVRPMutation.h \
    problems/VRPTW/moeoVRPInit.h \
    problems/VRPTW/moeoVRPEvalFunc.h \
    problems/VRPTW/moeoVRP.h \
    dummyPhase.h \
    VRPInstanceLoader.h \
    neighborhoods/staticNeighborhood.h \
    do_makes/make_neighborhood.h \
    problems/VRPTW/do_makes.h \
    core/eoSingleOp.h \
    core/eoQuadSingleGenOp.h \
    core/eoMonSingleGenOp.h \
    agents/moeoJFOPhaseAlgorithm.h \
    core/phase.h \
    agents/defaultCommunicationPhase.h \
    problems/VRPTW/moeoVRPIterSwap.h \
    core/moMOLS.h \
    core/moNeighborhoodExplorer.h \
    libs/conversions.h \
    core/moeoCoverageMetric.h
