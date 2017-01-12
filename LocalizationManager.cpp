/*Copyright 2016 Visual Entertainment And Technologies, LLC. All Rights Reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

- Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

- Neither the name of Visual Entertainment And Technologies, LLC
nor the names of its contributors may be used to endorse or promote products
derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.")*/


/* *
 * CODING GUIDELINES...
 * Allman Indent Style!
 * 100 +/- Column Width
 * Avoid Namespaces
 * Lower Camel Case for variables and functions.
 * Upper Camel Case for everything else. (UI Widgets, Class Names)
 * Don't shoot me if I forget these things as well...
 * */


#include "LocalizationManager.h"
#include <QtXml/QDomDocument>
#include <QDomNode>
#include <QFile>
#include <QMessageBox>

//Create localization manager
LocalizationManager::LocalizationManager(QString fileName, QWidget *widget)
{
    //Open the localization file
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::critical(widget,"File Error",
                              QString("Whopse, opening %1 failed.").arg(fileName));
             return;
    }

    QString errorMsg;
    int errorLine, errorColumn;
    QDomDocument doc;
    QDomElement wordElement;


    //Parse the xmlfile, if it's bad, pop up!
    if(!doc.setContent(&file,&errorMsg,&errorLine,&errorColumn))
    {
        QMessageBox::critical(widget,"Error Processing Localization File",
                             QString("Error Message: %1\nLine: %2\nColumn: %3").arg(
                             errorMsg,errorLine,errorColumn));
       return;
    }

    //Begin Reading and putting the data into the map
    QDomElement rootNode = doc.firstChildElement("Localization");
    if(rootNode.isNull())
    {
         QMessageBox::critical(widget,"Error","Localization is malformed.");
        return;
    }

    //Get first word, and lets go
    rootNode = doc.documentElement();
    wordElement = rootNode.firstChildElement("word");

    //loop through the xml file and insert into word map
    while(!wordElement.isNull())
    {
        wordMap.insert(wordElement.attributeNode("id").value().toInt(), wordElement.text());
        wordElement = wordElement.nextSiblingElement("word");
    }

    file.close();

}

//Get wordmap size.
int LocalizationManager::size()
{
    return wordMap.size();
}

//send ID and return translated word
QString LocalizationManager::getWord(int id)
{
    if(wordMap.contains(id))
        return wordMap.find(id).value();
    else
        return "";
}
