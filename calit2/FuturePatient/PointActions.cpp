#include "PointActions.h"

#include <cvrKernel/PluginHelper.h>
#include <cvrConfig/ConfigManager.h>
#include <PluginMessageType.h>

#include <osg/Matrix>

#include <iostream>

using namespace cvr;

PointActionPDF::PointActionPDF(std::string file)
{
    _file = file;

    size_t pos = file.find_last_of("\\/");
    if(pos != std::string::npos && pos != file.size()-1)
    {
	std::string basename;
	basename = file.substr(pos+1);
	_text = std::string("Open ") + basename;
    }
}

void PointActionPDF::action()
{
    std::cerr << "Calling PDF action on file: " << _file << std::endl;

    osg::Matrix m;
    osg::Vec3 pos = ConfigManager::getVec3("Plugin.FuturePatient.DefaultPDFPos");
    m.makeTranslate(pos);

    OsgPdfLoadRequest pdflr;
    pdflr.transform = m;
    pdflr.width = 1000.0;
    pdflr.path = _file;
    pdflr.loaded = false;
    pdflr.tiledWallObject = true;
    pdflr.object = NULL;

    PluginHelper::sendMessageByName("OsgPdf",PDF_LOAD_REQUEST,(char*)&pdflr);
}

const std::string & PointActionPDF::getActionText()
{
    return _text;
}
