#include "filesManager.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include <QObject>
//#include <QRandomGenerator>
#include <sstream>
#include <fstream>


filesManager::filesManager()
{
    MinValForLog = 1e+12;

}

filesManager::~filesManager()
{

}


// called from open buttons and install buttons
QString filesManager::GetChoosenDirFromDialog(QString targetDirFor, int type){

    QString chosen_Dir;

    /*
    targetDirFor = "Choose the "+ targetDirFor + " directory";


    if(type == 0){ // to select File
        chosen_Dir = QFileDialog::getOpenFileName(
                    this,
                    tr(targetDirFor.toStdString().c_str()),
                    QDir::currentPath(),
                    "All files (*.*)"//;;Text files (*.txt)" // extentions to show
                    );
    }
    else if(type == 1){ // to select Directory
        chosen_Dir = QFileDialog::getExistingDirectory(0, (targetDirFor), QDir::currentPath());
    }
    else{
        chosen_Dir = QFileDialog::getExistingDirectory(0, (targetDirFor), QDir::currentPath());
    }

    //QMessageBox::information(this, tr("File Path is : "), defaultFileName);

    if(chosen_Dir != NULL && chosen_Dir != "" ){

        showResultsOutput("the chosen file is " + chosen_Dir , 4);

    }else{
        //QMessageBox::information(this, tr("File..!!"),"No file is choosen");
        showResultsOutput("No file Was chosen, please choose an input file to fill the components automatically ! ", 4);

        chosen_Dir = "";
    }
*/
    return chosen_Dir;

}


// it's independent from mode (mt or seq) and it's related to the data in SAF.txt . it fill DataTables
QMap<QString,QMap<QString,QMap<QString,QMap<QString,QMap<QString,QMap<double,double>>>>>> filesManager::Read_final_result_file(QString FilePath, int TableOf){


    QTextStream(stdout) << "---------------- Read_final_result_file() ------------------" << "\n";
    showResultsOutput("Reading Result file... " + FilePath, 4);

    QMap<QString,QMap<QString,QMap<QString,QMap<QString,QMap<QString,QMap<double,double>>>>>> ResultTable;
    QMap<QString,QMap<QString,QMap<QString,QMap<QString,QMap<QString,QMap<double,double>>>>>> ErrorTables;

    QString line, Geometry, Quantity,SrcRegionName , organTargetname, RadTracerName , ParticleName , word;
    std::string Quantity1, Geometry1, SrcRegionName1 , organTargetname1,RadTracerName1 ,  ParticleName1 , word1;
    double mass, volume, density, particleE, Val, StandardDeviation, err, CompuTime;
    unsigned long long int ival;
    bool isADataLines = false ;


    StandartDeviation.clear();
    RegionParameterValueMap.clear();
    StandartDeviation.clear();
    ResultParticleSourceEnergyTime.clear();
    ResultQuantityGeometryRadioTracerSourceTargetValues.clear();


    std::ifstream fileR(FilePath.toStdString());

    if(fileR.is_open()){

        std::string linee = line.toStdString() ;

        // to fill the map array DataTables[SrcRegionName][organTargetname][ParticleName][particleE] = Val;
        while ( getline(fileR, linee)) {

            //G4cout << " the line " << line << G4"\n" ;

            std::istringstream LineString(linee);
            LineString >> word1;
            word = QString::fromStdString(word1);

            //G4cout << " the word " << word << G4"\n" ;

            if(isADataLines == true){

                if (word == "*") {

                    isADataLines = false;
                    continue;
                }
                if (word == "#") {
                    continue;
                }
                organTargetname = word;

                LineString >> Val;
                LineString >> StandardDeviation;
                LineString >> err;
                LineString >> ival; // for Num Steps
                LineString >> mass;
                LineString >> volume;
                LineString >> density;

                if(ParticleName == "RadioTracer"){

                    ResultQuantityGeometryRadioTracerSourceTargetValues[Quantity][Geometry][RadTracerName][SrcRegionName][organTargetname] = Val;
                    ResultQuantityGeometryRadioTracerSourceTargetStandardDeviation[Quantity][Geometry][RadTracerName][SrcRegionName][organTargetname] = StandardDeviation;
                    ResultQuantityGeometryRadioTracerSourceTargetRelativeStandardDeviation[Quantity][Geometry][RadTracerName][SrcRegionName][organTargetname] = err;

                    //QTextStream(stdout) << "------------------------ Quantity " << Quantity << " RadTracerName " << RadTracerName << " SrcRegionName " << SrcRegionName << " organTargetname " << organTargetname << " Val " << Val << "\n" ;
                }
                else{

                    //std::string jjjj; LineString >> jjjj; // this is the error, but because the % we have to write the double value of error in the file to save it in the map

                    ResultTable[Quantity][Geometry][ParticleName][SrcRegionName][organTargetname][particleE] = Val;
                    ErrorTables[Quantity][Geometry][ParticleName][SrcRegionName][organTargetname][particleE] = err;

                    StandartDeviation[Quantity][Geometry][ParticleName][SrcRegionName][organTargetname][particleE] = StandardDeviation;

                    bool isin = false;
                    for (int ss = 0 ; ss < ResEnergies[Quantity][Geometry][ParticleName].size(); ss++) {
                        if(ResEnergies[Quantity][Geometry][ParticleName][ss] == particleE){
                            isin = true;
                        }
                    }
                    if(isin == false){
                        ResEnergies[Quantity][Geometry][ParticleName].push_back(particleE);
                    }
                }

                RegionParameterValueMap[Geometry]["Mass"][organTargetname] = mass;
                RegionParameterValueMap[Geometry]["Volume"][organTargetname] = volume;
                RegionParameterValueMap[Geometry]["Density"][organTargetname] = density;

                if(MinValForLog > Val && Val != 0){
                    MinValForLog = Val;
                }
                //G4cout << "----------------------- " << organTargetname << " " << Val << " " << StandardDeviation << G4"\n";
            }

            if (word == "******" && isADataLines == false) {

                LineString >> Quantity1 >> SrcRegionName1 >> ParticleName1 ;

                //std::cout << " Quantity " << Quantity << " Geometry " << Geometry << " SrcRegionName " << SrcRegionName<< " ParticleName " << ParticleName  << std::"\n" ;

                Quantity = QString::fromStdString(Quantity1);
                ParticleName = QString::fromStdString(ParticleName1);
                SrcRegionName = QString::fromStdString(SrcRegionName1);

                if(ParticleName == "RadioTracer"){
                    LineString >> RadTracerName1 >> Geometry1 ;
                    RadTracerName = QString::fromStdString(RadTracerName1);
                    Geometry = QString::fromStdString(Geometry1);
                }
                else{
                    LineString >> particleE
                            >> Geometry1 >> word1 >> word1 >> word1 >> word1
                            >> word1 >> word1 >> word1 >> word1 >> word1
                            >> word1 >> word1 >> word1 >> word1 >> word1
                            >> word1 >> CompuTime ;

                    Geometry = QString::fromStdString(Geometry1);

                    //G4cout << " Quantity "<< Quantity << " SrcRegionName " << SrcRegionName << " ParticleName " << ParticleName << " particleE " << particleE  << " TotalEventNumber " << TotalEventNumber << G4"\n" ;
                    ResultParticleSourceEnergyTime[Quantity][Geometry][ParticleName][SrcRegionName][particleE] = CompuTime;
                }
                isADataLines = true;
            }
        }

        for ( auto Obeg = ResEnergies.begin(); Obeg != ResEnergies.end(); ++Obeg  )
        {
            for ( auto Abeg = Obeg.value().begin(); Abeg != Obeg.value().end(); ++Abeg  )
            {
                for ( auto Bbeg = Abeg.value().begin(); Bbeg != Abeg.value().end(); ++Bbeg  )
                {
                    std::sort(ResEnergies[Obeg.key()][Abeg.key()][Bbeg.key()].begin(), ResEnergies[Obeg.key()][Abeg.key()][Bbeg.key()].end());
                }
            }
        }
        fileR.close();
    }
    else{
        showResultsOutput("canno't open the file : "+FilePath , 4 );
    }
    if(TableOf == 1){
        return ResultTable;
    }else {
        return ErrorTables;
    }

}


// to read the comparison file (contains MIRD or ICRP or... data ) and create result_file and histogrammes files. it fill CompareDataTable, you have to take care about the number_of_energies variable and update it each time you change the CompareFile
QMap<QString,QMap<QString,QMap<QString,QMap<QString,QMap<QString,QMap<double,double>>>>>> filesManager::Read_Comparison_file(QString FilePath, int NumOfRefEne){

    QTextStream(stdout) << "---------------- Read_Comparison_file() ------------------" << "\n";
    showResultsOutput("Reading Reference file... " + FilePath, 4);

    // organSource, organTarget, particle , energy, SAF
    QMap<QString,QMap<QString,QMap<QString,QMap<QString,QMap<QString,QMap<double,double>>>>>> ReferenceTable ;

    QString line ,Geometry ,Quantity, SrcRegionName , organTargetname, ParticleName, RadTracerName, word;
    double Val;

    bool isADataLines = false ;

    std::string  ParticleName1 ,Geometry1, RadTracerName1, SrcRegionName1, organTargetname1 ,Quantity1, word1, QuantityUnit, EnergyUnit;
    std::ifstream fileR(FilePath.toStdString());

    ReferenceQuantityGeometryRadioTracerSourceTargetValues.clear();

    QVector< double > particleEnergies;

    if(fileR.is_open()){

        std::string linee = line.toStdString() ;

        // to fill the map array DataTables[SrcRegionName][organTargetname][ParticleName][particleE] = Val;
        while (getline(fileR, linee)) {

            std::istringstream LineString(linee);
            LineString >> word1;
            word = QString::fromStdString(word1);

            if (word == "******" && isADataLines == false) {

                LineString >> Quantity1 >> QuantityUnit >> SrcRegionName1 >> ParticleName1;
                //G4cout << " SrcRegionName " << SrcRegionName << " ParticleName " << ParticleName  << G4"\n" ;
                SrcRegionName = QString::fromStdString(SrcRegionName1);
                ParticleName = QString::fromStdString(ParticleName1);
                Quantity = QString::fromStdString(Quantity1);

                if(ParticleName == "RadioTracer"){
                    LineString >> RadTracerName1;
                    RadTracerName = QString::fromStdString(RadTracerName1);
                    LineString >> Geometry1;
                    Geometry = QString::fromStdString(Geometry1);
                }else{
                    LineString >> Geometry1 >> EnergyUnit ;
                    Geometry = QString::fromStdString(Geometry1);

                    double Enee;
                    int numWords = 0;
                    while (LineString >> Enee) {
                        if(EnergyUnit == "keV" || EnergyUnit == "KeV" || EnergyUnit == "KEV" || EnergyUnit == "kev") {Enee = Enee * 0.001;}
                        else if(EnergyUnit == "eV" || EnergyUnit == "EV" || EnergyUnit == "Ev" || EnergyUnit == "ev") {Enee = Enee * 0.000001;}
                        particleEnergies.push_back(Enee);
                        //G4cout << " Enee " << Enee<< G4"\n" ;
                        ++numWords;
                    }
                    NumOfRefEne = numWords;
                }

                isADataLines = true;
            }

            // read line of data that contains the name of target organ and the SAF correspondants
            if(isADataLines == true && word != "******"){

                if (word == "*") {

                    isADataLines = false;
                    continue;
                }

                if (word == "#") {
                    continue;
                }

                organTargetname = word;
                //std::cout << " - SrcRegionName " << SrcRegionName << " - organTargetname " << organTargetname << " - ParticleName " << ParticleName << std::"\n" ;
                if(ParticleName == "RadioTracer"){
                    LineString >> Val;
                    if(QuantityUnit == "rad/uCi-h" || QuantityUnit == "RAD/UCI-H" || QuantityUnit == "rad/uci-h") {Val = Val*1e-2/(37000000000*1e-6*3.6e+3);}
                    ReferenceQuantityGeometryRadioTracerSourceTargetValues[Quantity][Geometry][RadTracerName][SrcRegionName][organTargetname] = Val;
                    //QTextStream(stdout)  << " ----------------------- Quantity " << Quantity <<  " Geometry " << Geometry << " RadTracerName " << RadTracerName << " - SrcRegionName " << SrcRegionName << " - organTargetname " << organTargetname << " - val " << Val  << "\n";
                }
                else{

                    for( int zas = 0 ; zas < NumOfRefEne ; zas++ ){
                        LineString >> Val ;
                        if(Quantity == "SAF"){ if(QuantityUnit == "g-1" || QuantityUnit == "G-1" || QuantityUnit == "gram-1" || QuantityUnit == "Gram-1" || QuantityUnit == "GRAM-1") {Val = Val * 1000;}}
                        if(Quantity == "S"){if(QuantityUnit == "rad/uCi-h" || QuantityUnit == "RAD/UCI-H" || QuantityUnit == "rad/uci-h") {Val = Val*1e-2/(37000000000*1e-6*3.6e+3);}}
                        ReferenceTable[Quantity][Geometry][ParticleName][SrcRegionName][organTargetname][particleEnergies[zas]] = Val;

                        //G4cout << " - SrcRegionName " << SrcRegionName << " - organTargetname " << organTargetname << " - ParticleName " << ParticleName << " particleEnergies[zas] " << particleEnergies[zas] << " - Val " <<  Val  <<  G4"\n" ;
                        if(MinValForLog > Val && Val != 0){
                            MinValForLog = Val;
                        }

                        //std::cout << " - SrcRegionName " << SrcRegionName << " - organTargetname " << organTargetname << " - ParticleName " << ParticleName << " particleEnergies[zas] " << particleEnergies[zas] << " - Val " <<  Val  <<  std::"\n" ;
                    }
                }

            }
        }
        NumOfRefEne = particleEnergies.size();
        QTextStream(stdout) << "Filled Reference size " << ReferenceTable.size() << "\n";
    }
    else{
        showResultsOutput("Canno't Open Reference file... " + FilePath, 4);
    }

    return ReferenceTable ;
}


QMap<QString,QMap<QString,QMap<QString,QMap<double,double >>>> filesManager::Read_CrossSection_file(QString FilePath){

    QTextStream(stdout) << "---------------- " << __FUNCTION__ << " ------------------" << "\n";
    //showResultsOutput("Reading Cross Section file... " + FilePath, 4);

    QMap<QString,QMap<QString,QMap<QString,QMap<double,double >>>> ParticleMaterialProcessEneCrossSection ;// particle, material, process, Energy, Sigma

    std::ifstream file(FilePath.toStdString() , std::ios::binary);

    if(file.is_open()){

        QTextStream(stdout) << "\nReading file " << FilePath << "\n" ;

        //QString indicator, word, MatN, PartN, ProcN;//, line ;
        double Ene , CrosS;
        int procNum = 0 ;

        std::string line, word, MatN, PartN, ProcN, indicator ;

        QMap<QString,QVector<double>> ParticleProcessNames;
        QVector<QString> ProcessNames, Words;

        while (getline(file, line)) {

            //G4cout << " the line " << line << G4"\n" ;

            std::istringstream A(line);

            if(A.str().empty()){
                continue;
            }

            A >> word ;
            if(word == "Table"){
                indicator = word;
                A >> PartN >> MatN;
                //G4cout << "Data for Particle:"<< PartN << " and Material:" << MatN << G4"\n";
                ParticleProcessNames.clear();
                continue;
            }
            else if(word == "Energy(MeV)"){

                //G4cout << "The first is Energy(MeV)" << G4"\n";

                std::istringstream C(line);
                std::string LineWords ;
                ProcessNames.clear();
                Words.clear();


                while( C >> LineWords ){
                    if(LineWords.empty() || LineWords == ""){continue;}
                    else{
                        //G4cout << LineWords << " " << G4"\n";
                        Words.push_back(LineWords.c_str());
                    }
                }

                for (unsigned int k = 1; k < Words.size()-18; k++) {
                    //G4cout << LineWords << " " << G4"\n";
                    ProcessNames.push_back(Words[k]);
                }

                procNum = ProcessNames.size();
                //G4cout << "Number Of Process for particle "<< PartN << " and material " << MatN << " is " << procNum << G4"\n";

                continue;
            }
            else if(word == "Latex"){
                indicator = word;
                continue;
            }

            //G4cout << "\n" << G4"\n";

            std::istringstream B(line);
            if(indicator == "Table"){

                //G4cout << "indicator == Table " << G4"\n";

                double tt = 0. ;
                B >> Ene;
                for (unsigned int k = 0; k < ProcessNames.size(); k++) {
                    B >> ParticleMaterialProcessEneCrossSection[PartN.c_str()][MatN.c_str()][ProcessNames[k]][Ene];
                    tt += ParticleMaterialProcessEneCrossSection[PartN.c_str()][MatN.c_str()][ProcessNames[k]][Ene];
                    //showResultsOutput(" - Energy " + Ene +" - Process :  "+ ProcessNames[k] + " - MacroCrossSection " + ParticleMaterialProcessEneCrossSection[PartN.c_str()][MatN.c_str()][ProcessNames[k]][Ene] + " - Total " + tt ,4);
                }
            }
        }
    }


    return ParticleMaterialProcessEneCrossSection ;
}


//called from MainWindow class function ()
QVector<QString> filesManager::ReadTextFromFile(QString FilePathString){

    showResultsOutput("Reading text from file " + FilePathString, 4);

    QTextStream(stdout) << "Reading text from file " << FilePathString << "\n";

    QFile file(FilePathString);
    QVector<QString> lines;

    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());

    }else{

        QTextStream in(&file);

        while(!in.atEnd()) {
            QString line = in.readLine();

            //QStringList fields = line.split(QRegExp("(\\s|\\n|\\r)+"), QString::SkipEmptyParts);

            //QTextStream(stdout) << line + " " << "\n";
            lines.push_back(line);
        }

        file.close();
        showResultsOutput( "Closing file " + file.fileName(),4);

    }
    return lines;
}


QString filesManager::ReadTextFromFileInOneString(QString FilePathString){

    showResultsOutput("Reading text from file " + FilePathString, 4);

    //QTextStream(stdout) << "Reading text from file " << FilePathString << "\n";

    QFile file(FilePathString);
    QString lines = "";

    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());

    }else{

        QTextStream in(&file);
        QString line;

        while(!in.atEnd()) {
            line = in.readLine();
            lines += line; lines += "\n";
            showResultsOutput( line + " " , 4);
        }

        file.close();
        QTextStream(stdout) << "Closing file " << file.fileName() << "\n";

    }
    return lines;
}


//called from MainWindow class function ()
void filesManager::WriteTextToFile(QString FileName , QString TextToWrite){


    showResultsOutput("Writing Text to the File "+FileName, 4);
    showResultsOutput("The text to write is : "+TextToWrite , 4);

    // Create a new file
    QFile file(FileName);

    file.open(QIODevice::WriteOnly); // | QIODevice::Text);

    QTextStream stream(&file);
    //stream.setCodec("UTF-8");

    stream << TextToWrite ;

    // optional, as QFile destructor will already do it:
    file.close();

}


void filesManager::Read_ICRP110MasssSAFs107RadiationFiles(QString DataDirName ){

    ICRPSAFs.clear();
    ICRPRadioNuclideData.clear();
    ICRPRadioNuclideHalfLives.clear();
    SourceParticleEnergyValues.clear();
    ICRPRadioNuclideDataDiscSpec.clear();
    //ICRPRadioNuclideFSNData.clear();

    for (int zzz = 0 ; zzz < 17; zzz++) { // ICRP files  "< 14"

        QVector< double > particleEnergies;

        QString Geometry ,
                Quantity = "SAF",
                SrcRegionName ,
                organTargetname,
                ParticleName,
                RadioNuclideName,
                FilePathString,
                word;

        if(zzz==0){
            Geometry = "ICRPAdultMale";
            ParticleName = "gamma";
            FilePathString = "rcp-am_photon_2016-08-12.SAF";
        }
        else if(zzz==1){
            Geometry = "ICRPAdultMale";
            ParticleName = "e-";
            FilePathString = "rcp-am_electron_2016-08-12.SAF";
        }
        else if(zzz==3){
            Geometry = "ICRPAdultMale";
            ParticleName = "alpha";
            FilePathString = "rcp-am_alpha_2016-08-12.SAF";
        }
        else if(zzz==4){
            Geometry = "ICRPAdultMale";
            ParticleName = "neutron";
            FilePathString = "rcp-am_neutron_2016-08-12.SAF";
        }
        else if(zzz==5){
            Geometry = "ICRPAdultFemale";
            ParticleName = "gamma";
            FilePathString = "rcp-af_photon_2016-08-12.SAF";
        }
        else if(zzz==6){
            Geometry = "ICRPAdultFemale";
            ParticleName = "e-";
            FilePathString = "rcp-af_electron_2016-08-12.SAF";
        }
        else if(zzz==7){
            Geometry = "ICRPAdultFemale";
            ParticleName = "alpha";
            FilePathString = "rcp-af_alpha_2016-08-12.SAF";
        }
        else if(zzz==8){
            Geometry = "ICRPAdultFemale";
            ParticleName = "neutron";
            FilePathString = "rcp-af_neutron_2016-08-12.SAF";
        }
        else if(zzz==9){
            FilePathString = "sregions_2016-08-12.NDX";
        }
        else if(zzz==10){
            FilePathString = "torgans_2016-08-12.NDX";
        }
        else if(zzz==11){
            FilePathString = "ICRP-07.RAD";
        }
        else if(zzz==12){
            FilePathString = "ICRP-07.BET";
        }
        else if(zzz==13){
            FilePathString = "ICRP-07.ACK";
        }
        else if(zzz==14){
            FilePathString = "ICRP-07.NSF";
        }
        else if(zzz==15){
            FilePathString = "RadioPharmaceuticalsICRP.dat";
        }
        else if(zzz==16){
            FilePathString = "ICRP-07.NDX";
        }

        QString fm = DataDirName+"/"+FilePathString;
        QFile file(fm);

        if(!file.open(QIODevice::ReadOnly)) {
            //QMessageBox::information(0, "error", file.errorString());

        }else{

            QTextStream(stdout) <<zzz << " -Reading file: " << fm << "....\n";

            QTextStream in(&file);

            int lineInc = 0;
            int NumOfDataLines = 0;
            int DataLineInc = 0;
            int RadioNuclideDataInc = 0;
            int NumberOfEneRows = 0;
            double LastEnergy = 0;
            double Prob = 0;
            while(!in.atEnd()) {

                QRegExp space("\\s++");
                QString line = in.readLine().remove(space);

                //if(lineInc == 10000){
                //  break;
                //}

                if(!line.isEmpty()){

                    //QTextStream(stdout) << "line: " << line << "\n";

                    if(zzz<9){ // for SAFs files

                        //if(lineInc == 10000){ // energies line
                        //  return;
                        //}

                        if(lineInc == 3){ // energies line

                            QStringList fields = line.split(QRegExp("(\\s|\\n|\\r)+"), QString::SkipEmptyParts);

                            //QTextStream(stdout) << "Particle Energies" << "\n";

                            for (int ff = 2 ; ff < fields.size()-2; ff++) { // two data values of cutoff and ID
                                //                       //QTextStream(stdout) << "fields[ff] : " + fields[ff] << "\n";
                                particleEnergies.push_back(fields[ff].toDouble());
                                //QTextStream(stdout) << fields[ff].toDouble() << "\n";

                            }
                        }else if(lineInc > 4) { //begin of SAFs values

                            QString s1 = line.mid(0, 23);
                            line.remove(s1);

                            //QTextStream(stdout) << "s1 " << s1 << " line "<< line << "\n";

                            QStringList InputsVals = s1.remove(space).split("<-");
                            if(InputsVals.size() < 2 ){
                                break;
                            }else{
                                SrcRegionName = InputsVals[1].replace(" ","");
                                organTargetname = InputsVals[0].replace(" ","");
                                //if(SrcRegionName == "Liver" && organTargetname == "Liver"){
                                //QTextStream(stdout)  << " SrcRegionName " << SrcRegionName << " organTargetname " << organTargetname << "\n";
                                //}
                            }
                            QStringList fields = line.split(QRegExp("(\\s|\\n|\\r)+"), QString::SkipEmptyParts);
                            for (int ff = 0 ; ff < particleEnergies.size(); ff++) {


                                ICRPSAFs["SAF"][Geometry][ParticleName][particleEnergies[ff]][SrcRegionName][organTargetname] = fields[ff].toDouble();

                                //if(SrcRegionName == "Liver" && organTargetname == "Liver"){
                                    //QTextStream(stdout) << "Geometry " << Geometry <<  " ParticleName "<< ParticleName << " particleEnergies[ff] " << particleEnergies[ff] << " SrcRegionName " << SrcRegionName << " organTargetname " << organTargetname << " fields[ff].toDouble() "<< fields[ff].toDouble() << "\n";
                                //}
                                bool isin = false;
                                for (int dd = 0 ; dd < SourceParticleEnergyValues[SrcRegionName][ParticleName].size(); dd++) {
                                    if(particleEnergies[ff] == SourceParticleEnergyValues[SrcRegionName][ParticleName][dd]){isin = true;break;}}
                                if(isin == false){SourceParticleEnergyValues[SrcRegionName][ParticleName].push_back(particleEnergies[ff]);                                    }
                            }
                        }
                    }
                    else if(zzz==9 || zzz==10){ // for Masses data files

                        int nonelines = 4; int numchara = 12; //for zzz==9, source

                        if (line.split(QRegExp("(\\s|\\n|\\r)+"), QString::SkipEmptyParts).size() < 3){continue;}

                        if(zzz==10){if(lineInc > 84 ){break;}}
                        if(zzz==9){if(lineInc > 47 ){break;}}

                        if(zzz==10){nonelines = 3;numchara = 13;} //for zzz==10, target

                        if(lineInc > nonelines) {//begin of Masses values

                            QString s1 = line.mid(0, numchara);
                            line.remove(s1);
                            s1.replace(" ","");
                            //QTextStream(stdout) << "line " << line << " s1 " << s1 << "\n";

                            QStringList InputsVals = line.split(QRegExp("(\\s|\\n|\\r)+"), QString::SkipEmptyParts);

                            if(InputsVals.size() < 2 ){
                                continue;
                            }else{

                                if(zzz==9){ // source regions file
                                    //QTextStream(stdout) << " s1 " << s1 << " InputsVals[0] "<< InputsVals[0] << " InputsVals[2] "<< InputsVals[2] << "\n";
                                    RegionParameterValueMap["ICRPAdultMale"]["Mass"][s1] = InputsVals[0].toDouble();
                                    RegionParameterValueMap["ICRPAdultFemale"]["Mass"][s1] = InputsVals[2].toDouble();
                                }else if(zzz==10){ // target regions file
                                    //QTextStream(stdout) << " s1 " << s1 << " InputsVals[0] "<< InputsVals[0] << " InputsVals[1] "<< InputsVals[1] << "\n";

                                    if(RegionParameterValueMap["ICRPAdultMale"]["Mass"][s1] == 0.){
                                        RegionParameterValueMap["ICRPAdultMale"]["Mass"][s1] = InputsVals[0].toDouble();
                                        RegionParameterValueMap["ICRPAdultFemale"]["Mass"][s1] = InputsVals[1].toDouble();
                                    }
                                }
                            }
                        }
                    }
                    else if(zzz==11){ // for Radiation files

                        QStringList fields = line.split(QRegExp("(\\s|\\n|\\r)+"), QString::SkipEmptyParts);

                        if(fields.size() == 3){ // radionu data

                            RadioNuclideName = fields[0];

                            if(
                                    //positron emitters for PET
                                    RadioNuclideName != "F-18" &&
                                    RadioNuclideName != "O-15" &&
                                    RadioNuclideName != "C-11" &&
                                    RadioNuclideName != "N-13" /* &&

                                    //Targeted therapy with alpha emitters
                                    RadioNuclideName != "At-211" &&
                                    RadioNuclideName != "Bi-213" &&
                                    RadioNuclideName != "Ac-225" &&
                                    RadioNuclideName != "Ra-223" &&
                                    RadioNuclideName != "Th-227" &&
                                    //Spontaneous neutron emitters
                                    RadioNuclideName != "U-238" &&
                                    RadioNuclideName != "Pu-240" &&
                                    RadioNuclideName != "Fm-258" &&
                                    RadioNuclideName != "Es-254" &&
                                    RadioNuclideName != "Cm-244" &&
                                    RadioNuclideName != "Cf-252" &&
                                    // For cancer imaging
                                    RadioNuclideName != "I-131" &&
                                    RadioNuclideName != "Ga-68" &&
                                    RadioNuclideName != "Tc-99m"
                                    */
                                    ){
                                continue;
                            }

                            int HLConversion = 1;
                            QString HLunit = "s";
                            if(fields[1].contains("s")){HLConversion = 1;HLunit = "s";}
                            else if(fields[1].contains("m")){HLConversion = 60;HLunit = "m";}
                            else if(fields[1].contains("h")){HLConversion = 60*60;HLunit = "h";}
                            else if(fields[1].contains("d")){HLConversion = 60*60*24;HLunit = "d";}
                            else if(fields[1].contains("y")){HLConversion = 60*60*24*365;HLunit = "y";}

                            QTextStream(stdout) << "RadioNuclideName " << RadioNuclideName << " fields[1] " << fields[1]  << " HLunit " << HLunit << " HLConversion " << HLConversion << " ICRPRadioNuclideHalfLives[RadioNuclideName] "<< ICRPRadioNuclideHalfLives[RadioNuclideName] << "\n";

                            ICRPRadioNuclideHalfLives[RadioNuclideName] = HLConversion*fields[1].remove(HLunit).toDouble() ;

                        }else if(fields.size() == 4) { //radiation data ene yield par...

                            if(
                                    //positron emitters for PET
                                    RadioNuclideName != "F-18" &&
                                    RadioNuclideName != "O-15" &&
                                    RadioNuclideName != "C-11" &&
                                    RadioNuclideName != "N-13" /* &&

                                    //Targeted therapy with alpha emitters
                                    RadioNuclideName != "At-211" &&
                                    RadioNuclideName != "Bi-213" &&
                                    RadioNuclideName != "Ac-225" &&
                                    RadioNuclideName != "Ra-223" &&
                                    RadioNuclideName != "Th-227" &&
                                    //Spontaneous neutron emitters
                                    RadioNuclideName != "U-238" &&
                                    RadioNuclideName != "Pu-240" &&
                                    RadioNuclideName != "Fm-258" &&
                                    RadioNuclideName != "Es-254" &&
                                    RadioNuclideName != "Cm-244" &&
                                    RadioNuclideName != "Cf-252" &&
                                    // For cancer imaging
                                    RadioNuclideName != "I-131" &&
                                    RadioNuclideName != "Ga-68" &&
                                    RadioNuclideName != "Tc-99m"
                                    */
                                    ){
                                continue;
                            }

                            QStringList fields = line.split(QRegExp("(\\s|\\n|\\r)+"), QString::SkipEmptyParts);

                            
                            if(fields[3] == "G"){ParticleName = "gamma";}
                            else if(fields[3] == "PG"){ParticleName = "gamma";}
                            else if(fields[3] == "DG"){ParticleName = "gamma";}
                            else if(fields[3] == "X"){ParticleName = "gamma";}
                            else if(fields[3] == "AQ"){ParticleName = "gamma";}
                            else if(fields[3] == "B+"){ParticleName = "e+";}
                            else if(fields[3] == "B-"){ParticleName = "e-";}
                            else if(fields[3] == "BD"){ParticleName = "e-";}
                            else if(fields[3] == "IE"){ParticleName = "e-";}
                            else if(fields[3] == "AE"){ParticleName = "e-";}
                            else if(fields[3] == "A"){ParticleName = "alpha";}
                            else if(fields[3] == "AR"){ParticleName = "alpha";}
                            else if(fields[3] == "FF"){ParticleName = "FF";}
                            else if(fields[3] == "N"){ParticleName = "neutron";}

                            //QTextStream(stdout) << "RadioNuclideName " << RadioNuclideName << " fields[3] " << fields[3]  << " ParticleName " << ParticleName << " fields[2].toDouble() " << fields[2].toDouble() << " fields[1].toDouble() " << fields[1].toDouble() << "\n";

                            bool isin = false;
                            for (int dd = 0 ; dd < RadionuclidesParticles[RadioNuclideName].size(); dd++) {
                                if(fields[3] == RadionuclidesParticles[RadioNuclideName][dd]){isin = true;break;}}
                            if(isin == false){
                                RadionuclidesParticles[RadioNuclideName].push_back(fields[3]);
                            }

                            if(ParticleName != "e-" && ParticleName != "e+"){
                                ICRPRadioNuclideDataDiscSpec[RadioNuclideName][ParticleName]["Discrete"][fields[2].toDouble()] = fields[1].toDouble();
                            }

                            if(ParticleName=="e+"){ParticleName = "e-";};
                            ICRPRadioNuclideData[RadioNuclideName][ParticleName][fields[2].toDouble()] = fields[1].toDouble();

                        }
                    }
                    /*else if(zzz==12){ // for beta and electron Radiation files

                        QStringList fields = line.split(QRegExp("(\\s|\\n|\\r)+"), QString::SkipEmptyParts);

                        if(fields.size() == 2){ // radionu data

                            if(NumberOfEneRows == 0){ //
                                RadioNuclideName = fields[0];
                                NumberOfEneRows = fields[1].toInt();

                            }else if(RadioNuclideDataInc > NumberOfEneRows){

                                RadioNuclideName = fields[0];
                                NumberOfEneRows = fields[1].toInt();
                                RadioNuclideDataInc = 0;

                                //QTextStream(stdout) << "RadioNuclideName " << RadioNuclideName << " NumberOfEneRows " << NumberOfEneRows  << " RadioNuclideDataInc " << RadioNuclideDataInc  << "\n";
                            }
                            else{

                                if(fields[0].toDouble() == 0 && fields[1].toDouble() == 0){

                                }else{

                                    //QTextStream(stdout) << RadioNuclideDataInc << " RadioNuclideName " << RadioNuclideName << " ParticleName " << ParticleName << " RadioNuclideDataInc " << RadioNuclideDataInc << " fields[0] " << fields[0].toDouble()  << " fields[1] " << fields[1].toDouble()  << "\n";
                                    //ICRPRadioNuclideData[RadioNuclideName][ParticleName][fields[0].toDouble()] = fields[1].toDouble();

                                    //double Energy = LastEnergy + (fields[0].toDouble()-LastEnergy)*QRandomGenerator::global()->generateDouble();
                                    double Energy = fields[0].toDouble();
                                    Prob = fields[1].toDouble();

                                    //QTextStream(stdout) << RadioNuclideDataInc << " RadioNuclideName " << RadioNuclideName << " RadioNuclideDataInc " << RadioNuclideDataInc << " Random Energy " << Energy << " Prob " << Prob << " fields[0] " << fields[0].toDouble()  << " fields[1] " << fields[1].toDouble()  << "\n";

                                    if(
                                            //positron emitters for PET
                                            RadioNuclideName != "F-18" &&
                                            RadioNuclideName != "O-15" &&
                                            RadioNuclideName != "C-11" &&
                                            RadioNuclideName != "N-13" &&
                                            //Targeted therapy with alpha emitters
                                            RadioNuclideName != "At-211" &&
                                            RadioNuclideName != "Bi-213" &&
                                            RadioNuclideName != "Ac-225" &&
                                            RadioNuclideName != "Ra-223" &&
                                            RadioNuclideName != "Th-227" &&
                                            //Spontaneous neutron emitters
                                            RadioNuclideName != "U-238" &&
                                            RadioNuclideName != "Pu-240" &&
                                            RadioNuclideName != "Fm-258" &&
                                            RadioNuclideName != "Es-254" &&
                                            RadioNuclideName != "Cm-244" &&
                                            RadioNuclideName != "Cf-252" &&
                                            // For cancer imaging
                                            RadioNuclideName != "I-131" &&
                                            RadioNuclideName != "Ga-68" &&
                                            RadioNuclideName != "Tc-99m"
                                            ){
                                        continue;
                                    }

                                    ICRPRadioNuclideDataDiscSpec[RadioNuclideName]["e-"]["Spectrum"][Energy] = Prob;
                                    ICRPRadioNuclideDataDiscSpec[RadioNuclideName]["e+"]["Spectrum"][Energy] = Prob;
                                    LastEnergy = fields[0].toDouble();
                                }
                            }
                            RadioNuclideDataInc++;
                        }
                    }
                    *//*else if(zzz==13){ // for Auger elect...

                        QStringList fields = line.split(QRegExp("(\\s|\\n|\\r)+"), QString::SkipEmptyParts);

                        if(NumberOfEneRows == 0){ //
                            if(fields.size() == 3){ // radionu data
                                RadioNuclideName = fields[0];
                                NumberOfEneRows = fields[2].toInt();
                            }

                        }else if(NumberOfEneRows == RadioNuclideDataInc){

                            if(fields.size() == 3){
                                RadioNuclideName = fields[0];
                                NumberOfEneRows = fields[2].toInt();
                                RadioNuclideDataInc = 0;
                            }
                            //QTextStream(stdout) << "RadioNuclideName " << RadioNuclideName << " NumberOfEneRows " << NumberOfEneRows  << " RadioNuclideDataInc " << RadioNuclideDataInc  << "\n";

                        }else{

                            if(
                                    //positron emitters for PET
                                    RadioNuclideName != "F-18" &&
                                    RadioNuclideName != "O-15" &&
                                    RadioNuclideName != "C-11" &&
                                    RadioNuclideName != "N-13" &&
                                    //Targeted therapy with alpha emitters
                                    RadioNuclideName != "At-211" &&
                                    RadioNuclideName != "Bi-213" &&
                                    RadioNuclideName != "Ac-225" &&
                                    RadioNuclideName != "Ra-223" &&
                                    RadioNuclideName != "Th-227" &&
                                    //Spontaneous neutron emitters
                                    RadioNuclideName != "U-238" &&
                                    RadioNuclideName != "Pu-240" &&
                                    RadioNuclideName != "Fm-258" &&
                                    RadioNuclideName != "Es-254" &&
                                    RadioNuclideName != "Cm-244" &&
                                    RadioNuclideName != "Cf-252" &&
                                    // For cancer imaging
                                    RadioNuclideName != "I-131" &&
                                    RadioNuclideName != "Ga-68" &&
                                    RadioNuclideName != "Tc-99m"
                                    ){
                                continue;
                            }

                            //QTextStream(stdout) << RadioNuclideDataInc << " RadioNuclideName " << RadioNuclideName << " RadioNuclideDataInc " << RadioNuclideDataInc << " fields[0] " << fields[0].toDouble()  << " fields[1] " << fields[1].toDouble()  << "\n";
                            //ICRPRadioNuclideData[RadioNuclideName][ParticleName][fields[0].toDouble()] = fields[1].toDouble();
                            ICRPRadioNuclideDataDiscSpec[RadioNuclideName][ParticleName]["Discrete"][fields[0].toDouble()] = fields[1].toDouble();

                            RadioNuclideDataInc ++;
                        }
                    }
                    *//*else if(zzz==14){ // for Neutron spectrum fission file

                        QStringList fields = line.split(QRegExp("(\\s|\\n|\\r)+"), QString::SkipEmptyParts);
                            //QTextStream(stdout) << "size "<< fields.size() << " -line: " << line << "\n";

                        if((lineInc == 0) || (lineInc != 0 && NumOfDataLines == DataLineInc)){ // radionu data

                            RadioNuclideName = fields[0];
                            NumOfDataLines = fields[2].toInt();
                            DataLineInc = 0;
                            //QTextStream(stdout) << "\n\n\n#"<< RadioNuclideName << "\n/SourceData/setEventsInitialEneData MeV Spectrum 0.2222222 0.000 0.00E+00 ";

                            //QTextStream(stdout) << "RadioNuclideName " << RadioNuclideName << " fields[1].toDouble() " << fields[1].toDouble() << " fields[2].toDouble() " << fields[2].toDouble()  << " DataLineInc " << DataLineInc<< " NumOfDataLines " << NumOfDataLines << "\n";

                        }else{

                            //QTextStream(stdout) << "RadioNuclideName " << RadioNuclideName << " fields[0].toDouble() " << fields[0].toDouble() << " fields[1].toDouble() " << fields[1].toDouble() << " fields[2].toDouble() " << fields[2].toDouble()<< "\n";
                            //QTextStream(stdout) << fields[0].toDouble() << " " << fields[2].toDouble() << " " ;
                            if(NumOfDataLines-1 == DataLineInc){ // radionu data
                                //QTextStream(stdout) << fields[1].toDouble() << " 0 ";
                            }

                            if(
                                    //positron emitters for PET
                                    RadioNuclideName != "F-18" &&
                                    RadioNuclideName != "O-15" &&
                                    RadioNuclideName != "C-11" &&
                                    RadioNuclideName != "N-13" &&
                                    //Targeted therapy with alpha emitters
                                    RadioNuclideName != "At-211" &&
                                    RadioNuclideName != "Bi-213" &&
                                    RadioNuclideName != "Ac-225" &&
                                    RadioNuclideName != "Ra-223" &&
                                    RadioNuclideName != "Th-227" &&
                                    //Spontaneous neutron emitters
                                    RadioNuclideName != "U-238" &&
                                    RadioNuclideName != "Pu-240" &&
                                    RadioNuclideName != "Fm-258" &&
                                    RadioNuclideName != "Es-254" &&
                                    RadioNuclideName != "Cm-244" &&
                                    RadioNuclideName != "Cf-252" &&
                                    // For cancer imaging
                                    RadioNuclideName != "I-131" &&
                                    RadioNuclideName != "Ga-68" &&
                                    RadioNuclideName != "Tc-99m"
                                    ){
                                continue;
                            }

                            //ICRPRadioNuclideFSNData[RadioNuclideName][fields[0].toDouble()][fields[1].toDouble()] = fields[2].toDouble();
                            ICRPRadioNuclideData[RadioNuclideName]["neutron"][fields[0].toDouble()] = fields[2].toDouble();
                            ICRPRadioNuclideDataDiscSpec[RadioNuclideName]["neutron"]["Spectrum"][fields[0].toDouble()] = fields[2].toDouble();

                            DataLineInc++;

                            if(NumOfDataLines == DataLineInc){
                                ICRPRadioNuclideDataDiscSpec[RadioNuclideName]["neutron"]["Spectrum"][fields[1].toDouble()] = 0;
                            }
                        }
                    }*/
                    else if(zzz==15){ // for Radiation files

                        std::string c1; std::string c2; std::string c3; double val;
                        std::string line1 = line.toStdString();
                        std::istringstream LineString(line1);

                        if(LineString.str().empty()){
                            continue;
                        }

                        LineString  >> c1 ;
                        LineString  >> c2 ;

                        RadiotracerradionucleidMap[c1.c_str()] = c2.c_str();

                        while(LineString >> c3 ){
                           LineString >> val;
                           RadioTracerSourceOrganResidenceTime[c1.c_str()][c2.c_str()][c3.c_str()] = val;
                        }

                    }
                }
                lineInc++;
            }
            file.close();
            //QTextStream(stdout) << "Closing file " << file.fileName() << "\n";
        }
    }
}


QMap<QString,QString> filesManager::ReadLinesFromFileWithFirstWordIndicator(QString FilePathString){

    showResultsOutput("Reading text from file " + FilePathString, 4);

    //QTextStream(stdout) << "Reading text from file " << FilePathString << "\n";

    QFile file(FilePathString);
    QMap < QString , QString > lines;

    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());

    }else{

        QTextStream in(&file);

        while(!in.atEnd()) {
            QRegExp space("\\s++");
            QString line = in.readLine().remove(space);

            if( !line.isEmpty()){

                QStringList fields = line.split(QRegExp("(\\s|\\n|\\r)+"), QString::SkipEmptyParts);
                QString Value = "";
                //fields.removeOne("");
                QString spacee = " ";
                int cc = fields.size()-1;
                for (int ff = 1 ; ff < fields.size(); ff++) {
                    if(fields[ff]=="" || fields[ff]==" "){
                        continue;
                    }
                    if(ff == cc){
                        spacee = "";
                    }
                    Value += fields[ff]+spacee ;
                }

                if(fields.size() > 0){
                    showResultsOutput( "Command : " + fields[0] + "  Value : "+ Value ,4);
                    lines[fields[0]] = Value;
                }
            }
        }

        file.close();
        QTextStream(stdout) << "Closing file " << file.fileName() << "\n";

    }
    return lines;
}


QMap<QString,QVector<QString>> filesManager::ReadLinesFromFileWithFirstWordIndicatorAndVector(QString FilePathString){

    showResultsOutput("Reading text from file " + FilePathString, 4);

    //QTextStream(stdout) << "Reading text from file " << FilePathString << "\n";

    QFile file(FilePathString);
    QMap < QString , QVector<QString> > lines;

    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());

    }else{

        QTextStream in(&file);

        while(!in.atEnd()) {
            QRegExp space("\\s++");
            QString line = in.readLine().remove(space);

            if( line.isEmpty()){

            }
            else{
                QStringList fields = line.split(QRegExp("(\\s|\\n|\\r)+"), QString::SkipEmptyParts);
                QString Value = "";
                //fields.removeOne("");
                QString spacee = " ";
                int cc = fields.size()-1;
                for (int ff = 1 ; ff < fields.size(); ff++) {
                    if(fields[ff]=="" || fields[ff]==" "){
                        continue;
                    }
                    if(ff == cc){
                        spacee = "";
                    }
                    Value += fields[ff]+spacee ;
                }

                if(fields.size() > 0){
                    showResultsOutput( "Command : " + fields[0] + "  Value : "+ Value ,1);
                    lines[fields[0]].push_back(Value);
                }
            }
        }

        file.close();
        QTextStream(stdout) << "Closing file " << file.fileName() << "\n";

    }
    return lines;
}


QMap<double,double> filesManager::ReadLinesFromFileWithFirstDoubleValueIndicator(QString FilePathString){

    showResultsOutput("Reading text from file " + FilePathString, 4);

    //QTextStream(stdout) << "Reading text from file " << FilePathString << "\n";

    QFile file(FilePathString);
    QMap < double , double > lines;

    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());

    }else{

        QTextStream in(&file);

        while(!in.atEnd()) {
            QRegExp space("\\s++");
            QString line = in.readLine().remove(space);

            if( line.isEmpty()){

            }
            else{
                QStringList fields = line.remove(space).split(" ");
                //fields.removeOne("");
                if(fields.size() > 0){
                    showResultsOutput( "fields[0] : " + fields[0] + "  fields[1] : "+ fields[1],4);
                    lines[fields[0].toDouble()] = fields[1].toDouble();
                }
            }
        }

        file.close();
        QTextStream(stdout) << "Closing file " << file.fileName() << "\n";

    }
    return lines;
}


QVector< QPair<QString,QString>> filesManager::ReadTextFromFileInQStringList(QString FilePathString){

    showResultsOutput("Reading text from file " + FilePathString, 4);

    //QTextStream(stdout) << "Reading text from file " << FilePathString << "\n";

    QFile file(FilePathString);
    QMap < QString , QString > lines;
    QVector< QPair<QString,QString>> CommandValuePair;

    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());

    }else{

        QStringList Commandslist;

        QTextStream in(&file);

        while(!in.atEnd()) {
            QRegExp space("\\s++");
            QString line = in.readLine().remove(space);

            if( line.isEmpty()){

            }
            else{
                QStringList fields = line.split(QRegExp("(\\s|\\n|\\r)+"), QString::SkipEmptyParts);
                QString Value = "";
                //fields.removeOne("");
                QString spacee = " ";
                int cc = fields.size()-1;
                for (int ff = 1 ; ff < fields.size(); ff++) {
                    if(fields[ff]=="" || fields[ff]==" "){
                        continue;
                    }
                    if(ff == cc){
                        spacee = "";
                    }
                    Value += fields[ff]+spacee ;
                }

                if(fields.size() > 0){
                    showResultsOutput( "Command : " + fields[0] + "  Value : " + Value, 4);
                    CommandValuePair.push_back(qMakePair(fields[0], Value));
                }
            }
        }

        file.close();
        QTextStream(stdout) << "Closing file " << file.fileName() << "\n";

    }
    return CommandValuePair;

}


QVector< QPair<QString,QString>> filesManager::ReadTextFromQStringInQStringList(QString sstring){


    QVector< QPair<QString,QString>> CommandValuePair;

    QTextStream in(&sstring);

    while(!in.atEnd()) {
        QRegExp space("\\s++");
        QString line = in.readLine().remove(space);

        if( line.isEmpty()){

        }
        else{
            QStringList fields = line.split(QRegExp("(\\s|\\n|\\r)+"), QString::SkipEmptyParts);
            QString Value = "";
            //fields.removeOne("");
            QString spacee = " ";
            int cc = fields.size()-1;
            for (int ff = 1 ; ff < fields.size(); ff++) {
                if(fields[ff]=="" || fields[ff]==" "){
                    continue;
                }
                if(ff == cc){
                    spacee = "";
                }
                Value += fields[ff]+spacee ;
            }

            if(fields.size() > 0){
                showResultsOutput("Command : " + fields[0] + "  Value : " + Value,4 );
                CommandValuePair.push_back(qMakePair(fields[0], Value));
            }
        }
    }

    return CommandValuePair;

}

QMap<QString,QMap<QString,double>> filesManager::ReadRegionsData(QString FilePathString){

    QMap<QString,QMap<QString,double>> RegionParameterValueMap;

    std::ostringstream filename1;
    std::ifstream file1(FilePathString.toStdString() , std::ios::binary);

    if(file1.is_open()){

        QTextStream(stdout) << "\nOpen file " << filename1.str().c_str() << "\n" ;

        std::string line , indicator, word;
        while (getline(file1, line)) {

            //G4cout << " the line " << line << G4"\n" ;

            if(line == ">> Regions Data"){
                //G4cout << "\n\n\n\n breaking because we reach >>" <<G4"\n";
                continue;
            }
            if(line == ">> Inputs Data"){
                //G4cout << "\n\n\n\n breaking because we reach >>" <<G4"\n";
                break;
            }

            std::istringstream LineString(line);

            if(LineString.str().empty()){
                continue;
            }

            std::istringstream AAA(LineString.str());
            AAA  >> word ;
            if(word == "#"){
                continue;
            }

            QString OrganName, Para;
            double X, Y, Z;
            int ik = 0;
            const std::string & Organ_line = LineString.str();
            char delim = ' ';
            std::stringstream ss (Organ_line);
            std::string item;

            while (getline (ss, item, delim)) {
                std::string ii = item;
                if(ii=="" || ii==" " || ii.empty() ){
                    continue;
                }

                if(ii=="" || ii==" " || ii.empty() ){
                    continue;
                }
                if(ik==0){
                    OrganName = item.c_str();

                }
                else if(ik==1) { // density
                    RegionParameterValueMap[OrganName]["Mass"] = atof(item.c_str());

                }
                else if(ik==2) {
                    RegionParameterValueMap[OrganName]["Density"] = atof(item.c_str());

                }
                else if(ik==3) {
                    RegionParameterValueMap[OrganName]["Volume"] = atof(item.c_str());

                }
                else if(ik==4) {
                    X=atof(item.c_str());
                    RegionParameterValueMap[OrganName]["X"] = X;
                }
                else if(ik==5) {
                    Y=atof(item.c_str());
                    RegionParameterValueMap[OrganName]["Y"] = Y;
                }
                else if(ik==6) {
                    Z=atof(item.c_str());
                    RegionParameterValueMap[OrganName]["Z"] = Z;
                    //QTextStream(stdout) << X << " " << Y << " " << Z << "\n";

                }
                else{

                }
                ik++;
            }
            //G4cout << OrganNameMassMap.size() << " -OrganName " << OrganName << " -mass " << OrganNameMassMap[OrganName] << " -density " << OrganNameDensityMap[OrganName] << G4"\n" ;
        }

        file1.close();
    }
    else{

        QTextStream(stdout) << "cannot open the file " << "\n" ;
    }

    return RegionParameterValueMap ;
}


void filesManager::showResultsOutput(QString text, int level){

    if(level == 1){
        QTextStream(stdout) << text << "\n";
    }
    else if(level == 2){ //for
        QTextStream(stdout) << text << "\n";
    }
}

