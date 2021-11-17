find_package(Boost COMPONENTS numpy)

PKG(GenQPDFAlg
    DEPENDS
        offline::SimEventV2
        offline::RecEvent
        offline::ElecEvent
        offline::CalibEvent
        offline::Identifier
        offline::Geometry
        offline::DataRegistritionSvc
        offline::BufferMemMgr
        offline::PMTCalibSvc
        Boost::filesystem
        Boost::system
)
