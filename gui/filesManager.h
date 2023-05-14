#ifndef FILESMANAGER_H
#define FILESMANAGER_H

#include <QVector>
#include <QString>
#include <QTextEdit>
#include <QDialog>

class filesManager
{
public:
    filesManager();
    ~filesManager();

    QString ReadTextFromFileInOneString(QString);
    QVector<QString> ReadTextFromFile(QString ) ;
    QMap<QString,QString> ReadLinesFromFileWithFirstWordIndicator(QString ) ;
    QMap<QString,QVector<QString>> ReadLinesFromFileWithFirstWordIndicatorAndVector(QString ) ;
    QMap<double,double> ReadLinesFromFileWithFirstDoubleValueIndicator(QString);
    QVector< QPair<QString,QString>> ReadTextFromFileInQStringList(QString) ;
    QVector< QPair<QString,QString>> ReadTextFromQStringInQStringList(QString) ;
    QMap<QString,QMap<QString,QMap<QString,QMap<QString,QMap<QString,QMap<double,double>>>>>> Read_final_result_file(QString,int);
    QMap<QString,QMap<QString,QMap<QString,QMap<QString,QMap<QString,QMap<double,double>>>>>> Read_Comparison_file(QString,int);
    //QMap<QString,QMap<QString,QMap<QString,QMap<QString,QMap<QString,QMap<double,double>>>>>> Read_ICRP110_SAFs(QString,int);
    //QMap<QString,QMap<QString,QMap<QString,QMap<QString,QMap<QString,QMap<double,double>>>>>> Read_ICRP110_Masses(QString,int);
    //QMap<QString,QMap<QString,QMap<QString,QMap<QString,QMap<QString,QMap<double,double>>>>>> Read_ICRP107_Radiation(QString,int);
    void Read_ICRP110MasssSAFs107RadiationFiles(QString);
    QMap<QString,QMap<QString,QMap<QString,QMap<double,double >>>> Read_CrossSection_file(QString);
    QMap<QString,QMap<QString,double>> ReadRegionsData(QString ) ;

    QMap<QString,QMap<QString,QMap<QString,QMap<QString,QMap<QString,QMap<double,double>>>>>> StandartDeviation;
    QMap<QString,QMap<QString,QMap<QString,QMap<QString,QMap<QString,QMap<double,double>>>>>> getStandartDeviationMap(){return StandartDeviation ;}

    void WriteTextToFile(QString FileName , QString TextToWrite) ;
    void showResultsOutput(QString, int);


    // called from open buttons and install buttons
    QString GetChoosenDirFromDialog(QString, int);

    QMap<QString,QMap<QString,QMap<QString,QMap<QString,QMap<double,double>>>>> getResultParticleSourceEnergyTime(){return ResultParticleSourceEnergyTime ;}
    QMap<QString,QMap<QString,QMap<QString,QVector<double>>>> getResEnergies(){return ResEnergies ;}
    double getMinValForLog(){return MinValForLog ;}

    QMap<QString,QMap<QString,QMap<QString,QMap<QString,QMap<QString,double>>>>> ResultQuantityGeometryRadioTracerSourceTargetValues ;
    QMap<QString,QMap<QString,QMap<QString,QMap<QString,QMap<QString,double>>>>> getResultQuantityGeometryRadioTracerSourceTargetValues(){return ResultQuantityGeometryRadioTracerSourceTargetValues ;}

    QMap<QString,QMap<QString,QMap<QString,QMap<QString,QMap<QString,double>>>>> ResultQuantityGeometryRadioTracerSourceTargetStandardDeviation ;
    QMap<QString,QMap<QString,QMap<QString,QMap<QString,QMap<QString,double>>>>> getResultQuantityGeometryRadioTracerSourceTargetStandardDeviation(){return ResultQuantityGeometryRadioTracerSourceTargetStandardDeviation ;}

    QMap<QString,QMap<QString,QMap<QString,QMap<QString,QMap<QString,double>>>>> ResultQuantityGeometryRadioTracerSourceTargetRelativeStandardDeviation ;
    QMap<QString,QMap<QString,QMap<QString,QMap<QString,QMap<QString,double>>>>> getResultQuantityGeometryRadioTracerSourceTargetRelativeStandardDeviation(){return ResultQuantityGeometryRadioTracerSourceTargetRelativeStandardDeviation ;}

    QMap<QString,QMap<QString,QMap<QString,QMap<QString,QMap<QString,double>>>>> ReferenceQuantityGeometryRadioTracerSourceTargetValues ;
    QMap<QString,QMap<QString,QMap<QString,QMap<QString,QMap<QString,double>>>>> getReferenceQuantityGeometryRadioTracerSourceTargetValues(){return ReferenceQuantityGeometryRadioTracerSourceTargetValues ;}

    QMap<QString,QMap<QString,QMap<QString,double>>> RegionParameterValueMap;
    QMap<QString,QMap<QString,QMap<QString,double>>> getRegionParameterValueMap(){return RegionParameterValueMap ;}

    //QVector<QString> getOrganNamesToScore(){return OrganNamesToScore ;}

    QMap<QString,QMap<QString,QMap<QString,QMap<double,QMap<QString,QMap<QString,double>>>>>> getICRPSAFs(){return ICRPSAFs ;}
    QMap<QString,QMap<QString,QMap<double,double>>> getICRPRadioNuclideData(){return ICRPRadioNuclideData ;}
    QMap<QString,QMap<QString,QMap<QString,QMap<double,double>>>> getICRPRadioNuclideDataDiscSpec(){return ICRPRadioNuclideDataDiscSpec ;}
    QMap<QString,double> getICRPRadioNuclideHalfLives(){return ICRPRadioNuclideHalfLives ;}
    QMap<QString,QVector<QString>> getRadionuclidesParticles(){return RadionuclidesParticles ;}
    std::map<QString,std::map<QString, std::vector<double>>> getSourceParticleEnergyValues(){return SourceParticleEnergyValues ;}

private:
    double MinValForLog;
    QMap<QString,QMap<QString,QMap<QString,QMap<QString,QMap<double,double>>>>> ResultParticleSourceEnergyTime;
    QMap<QString,QMap<QString,QMap<QString,QVector<double>>>> ResEnergies ;
    //QVector<QString> OrganNamesToScore ;

    QMap<QString,QMap<QString,QMap<QString,QMap<double,QMap<QString,QMap<QString,double>>>>>> ICRPSAFs ;
    QMap<QString,QMap<QString,QMap<double,double>>> ICRPRadioNuclideData ; // RadioNuclide Particle MonoOrSpectrum MonoEnergyOrSpectumEnergy YieldForMonoEnergyOrSpectumEnergy
    QMap<QString,QMap<double,QMap<double,double>>> ICRPRadioNuclideFSNData ; // RadioNuclide Particle MonoOrSpectrum MonoEnergyOrSpectumEnergy YieldForMonoEnergyOrSpectumEnergy
    QMap<QString,QMap<QString,QMap<QString,QMap<double,double>>>> ICRPRadioNuclideDataDiscSpec ; // RadioNuclide Particle MonoOrSpectrum MonoEnergyOrSpectumEnergy Min Max or (Mono Mono) YieldForMonoEnergyOrSpectumEnergy
    QMap<QString,double> ICRPRadioNuclideHalfLives ;
    QMap<QString,QVector<QString>> RadionuclidesParticles ;
    std::map<QString,std::map<QString, std::vector<double>>> SourceParticleEnergyValues ;

};


#endif // FILESMANAGER_H
