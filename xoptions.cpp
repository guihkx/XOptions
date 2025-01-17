/* Copyright (c) 2020-2024 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "xoptions.h"

XOptions::XOptions(QObject *pParent) : QObject(pParent)
{
#ifdef QT_GUI_LIB
    g_pRecentFilesMenu = nullptr;
    g_pCodePagesMenu = nullptr;
#endif
    g_bIsNeedRestart = false;
    g_nMaxRecentFilesCount = N_MAX_RECENT_FILES_COUNT;
    g_sName = QString("%1.ini").arg(qApp->applicationName());  // default name
    //    qRegisterMetaType<QVector<int>>("QVector<int>"); // Windows debug messages

    // #ifdef QT_DEBUG
    //     getBundle();
    // #endif
}

void XOptions::resetToDefault()
{
    qint32 nNumberOfRecords = g_listValueIDs.count();

    for (qint32 i = 0; i < nNumberOfRecords; i++) {
        XOptions::ID id = g_listValueIDs.at(i);

        bool bInsert = false;

        if ((id != ID_NU_LASTDIRECTORY) && (id != ID_NU_RECENTFILES)) {
            bInsert = true;
        }

        if (bInsert) {
            g_mapValues.insert(id, g_mapDefaultValues.value(id));
        }
    }
}

void XOptions::setValueIDs(const QList<ID> &listVariantIDs)
{
    g_listValueIDs = listVariantIDs;
}

void XOptions::setDefaultValues(QMap<XOptions::ID, QVariant> mapDefaultValues)
{
    g_mapDefaultValues = mapDefaultValues;
}

void XOptions::addID(ID id, QVariant varDefaultValue)
{
    g_listValueIDs.append(id);

    if (varDefaultValue.isValid()) {
        g_mapDefaultValues.insert(id, varDefaultValue);
    }
}

void XOptions::removeID(ID id)
{
    g_listValueIDs.removeOne(id);
    g_mapDefaultValues.remove(id);
    g_mapValues.remove(id);
}

XOptions::GROUPID XOptions::getGroupID(ID id)
{
    GROUPID result = GROUPID_UNKNOWN;

    switch (id) {
        case ID_VIEW_STAYONTOP:
        case ID_VIEW_STYLE:
        case ID_VIEW_QSS:
        case ID_VIEW_LANG:
        case ID_VIEW_SINGLEAPPLICATION:
        case ID_VIEW_SHOWLOGO:
        case ID_VIEW_FONT:
        case ID_VIEW_ADVANCED:
        case ID_VIEW_SELECTSTYLE: result = GROUPID_VIEW; break;
        case ID_FILE_SAVELASTDIRECTORY:
        case ID_FILE_SAVERECENTFILES:
        case ID_FILE_SAVEBACKUP:
        case ID_FILE_CONTEXT: result = GROUPID_FILE; break;
        case ID_SCAN_SCANAFTEROPEN:
        case ID_SCAN_RECURSIVE:
        case ID_SCAN_DEEP:
        case ID_SCAN_HEURISTIC:
        case ID_SCAN_VERBOSE:
        case ID_SCAN_ALLTYPES:
        case ID_SCAN_ENGINE:
        case ID_SCAN_DATABASEPATH:
        case ID_SCAN_CUSTOMDATABASEPATH:
        case ID_SCAN_YARARULESPATH:
        case ID_SCAN_EDITORFONT: result = GROUPID_SCAN; break;
        case ID_SIGNATURES_PATH: result = GROUPID_SIGNATURES; break;
        case ID_INFO_PATH: result = GROUPID_INFO; break;
        case ID_DISASM_FONT:
        case ID_DISASM_SYNTAX:
        case ID_DISASM_ADDRESSCOLON:
        case ID_DISASM_UPPERCASE:
        case ID_DISASM_HIGHLIGHT:
        case ID_DISASM_COLOR_ARROWS:
        case ID_DISASM_COLOR_ARROWS_SELECTED:
        case ID_DISASM_COLOR_REGS:
        case ID_DISASM_COLOR_NUMBERS:
        case ID_DISASM_COLOR_OPCODE:
        case ID_DISASM_COLOR_REFS:
        case ID_DISASM_COLOR_X86_REGS_GENERAL:
        case ID_DISASM_COLOR_X86_REGS_STACK:
        case ID_DISASM_COLOR_X86_REGS_SEGMENT:
        case ID_DISASM_COLOR_X86_REGS_DEBUG:
        case ID_DISASM_COLOR_X86_REGS_IP:
        case ID_DISASM_COLOR_X86_REGS_FLAGS:
        case ID_DISASM_COLOR_X86_REGS_FPU:
        case ID_DISASM_COLOR_X86_REGS_XMM:
        case ID_DISASM_COLOR_X86_OPCODE_CALL:
        case ID_DISASM_COLOR_X86_OPCODE_RET:
        case ID_DISASM_COLOR_X86_OPCODE_JCC:
        case ID_DISASM_COLOR_X86_OPCODE_PUSH:
        case ID_DISASM_COLOR_X86_OPCODE_POP:
        case ID_DISASM_COLOR_X86_OPCODE_NOP:
        case ID_DISASM_COLOR_X86_OPCODE_JMP:
        case ID_DISASM_COLOR_X86_OPCODE_INT3:
        case ID_DISASM_COLOR_X86_OPCODE_SYSCALL:
        case ID_DISASM_COLOR_ARM_REGS_GENERAL:
        case ID_DISASM_COLOR_ARM_OPCODE_B:
        case ID_DISASM_COLOR_ARM_OPCODE_BL:
        case ID_DISASM_COLOR_ARM_OPCODE_RET:
        case ID_DISASM_COLOR_ARM_OPCODE_PUSH:
        case ID_DISASM_COLOR_ARM_OPCODE_POP:
        case ID_DISASM_COLOR_ARM_OPCODE_NOP: result = GROUPID_DISASM; break;
        case ID_DEBUGGER_COLOR_BREAKPOINT: result = GROUPID_DEBUGGER; break;
        case ID_HEX_FONT:
        case ID_HEX_ADDRESSCOLON: result = GROUPID_HEX; break;
        case ID_STACK_FONT:
        case ID_STACK_ADDRESSCOLON: result = GROUPID_STACK; break;
        case ID_REGISTERS_FONT: result = GROUPID_REGISTERS; break;
        case ID_IODRIVER_FILENAME:
        case ID_IODRIVER_SERVICENAME: result = GROUPID_IODRIVER; break;
        default:
#ifdef QT_DEBUG
            qDebug("Unknown GroupID");
#endif
            result = GROUPID_UNKNOWN;
    }

    return result;
}

bool XOptions::isIDPresent(ID id)
{
    bool bResult = false;

    qint32 nNumberOfRecords = g_listValueIDs.count();

    for (qint32 i = 0; i < nNumberOfRecords; i++) {
        if (g_listValueIDs.at(i) == id) {
            bResult = true;
            break;
        }
    }

    return bResult;
}

bool XOptions::isGroupIDPresent(GROUPID groupID)
{
    bool bResult = false;

    qint32 nNumberOfRecords = g_listValueIDs.count();

    for (qint32 i = 0; i < nNumberOfRecords; i++) {
        if (getGroupID(g_listValueIDs.at(i)) == groupID) {
            bResult = true;
            break;
        }
    }

    return bResult;
}

bool XOptions::isNative()
{
    return checkNative(g_sName);
}

bool XOptions::isAppImage()
{
    bool bResult = false;

    QString sApplicationDirPath = qApp->applicationDirPath();

    bResult = (sApplicationDirPath.contains("/tmp/.mount_"));  // TODO more check

    return bResult;
}

void XOptions::setName(const QString &sValue)
{
    g_sName = sValue;
#ifdef QT_DEBUG
#ifdef Q_OS_WIN
#ifndef Q_OS_WIN64
    g_sName += "win32.debug.ini";
#else
    g_sName += "win64.debug.ini";
#endif
#endif
#ifdef Q_OS_LINUX
    g_sName += "linux.debug.ini";
#endif
#ifdef Q_OS_MACOS
    g_sName += "macos.debug.ini";
#endif
#endif
}

void XOptions::load()
{
    QSettings *pSettings = nullptr;

    bool bIsNative = isNative();

    if (bIsNative) {
        pSettings = new QSettings;
    } else {
        pSettings = new QSettings(getApplicationDataPath() + QDir::separator() + QString("%1").arg(g_sName), QSettings::IniFormat);
    }

#ifdef QT_DEBUG
    if (pSettings) {
        qDebug("XOptions load file %s", pSettings->fileName().toUtf8().data());
    }
#endif

    qint32 nNumberOfIDs = g_listValueIDs.count();

    bool bSaveLastDirectory = false;
    bool bLastDirectory = false;
    bool bSaveRecentFiles = false;
    bool bRecentFiles = false;

    for (qint32 i = 0; i < nNumberOfIDs; i++) {
        if (g_listValueIDs.at(i) == ID_FILE_SAVELASTDIRECTORY) {
            bSaveLastDirectory = true;
        } else if (g_listValueIDs.at(i) == ID_NU_LASTDIRECTORY) {
            bLastDirectory = true;
        } else if (g_listValueIDs.at(i) == ID_FILE_SAVERECENTFILES) {
            bSaveRecentFiles = true;
        } else if (g_listValueIDs.at(i) == ID_NU_RECENTFILES) {
            bRecentFiles = true;
        }
    }

    if (bSaveLastDirectory && (!bLastDirectory)) {
        g_listValueIDs.append(ID_NU_LASTDIRECTORY);
    }

    if (bSaveRecentFiles && (!bRecentFiles)) {
        g_listValueIDs.append(ID_NU_RECENTFILES);
    }

    nNumberOfIDs = g_listValueIDs.count();

    for (qint32 i = 0; i < nNumberOfIDs; i++) {
        ID id = g_listValueIDs.at(i);
        QString sName = idToString(id);

        QVariant varDefault;

        if (g_mapDefaultValues.contains(id)) {
            varDefault = g_mapDefaultValues.value(id);
        } else {
            // TODO remove,use addID
            switch (id) {
                case ID_ROOTPATH: varDefault = ""; break;
                case ID_DATAPATH: varDefault = "$data/data"; break;
                case ID_JSON: varDefault = ""; break;
                case ID_STRUCTSPATH: varDefault = "$data/structs"; break;
                case ID_STRUCTS_PATH: varDefault = "$data/structs"; break;
                case ID_AUTHUSER: varDefault = ""; break;
                case ID_AUTHTOKEN: varDefault = ""; break;
                case ID_NU_RECENTFILES: varDefault = QList<QVariant>(); break;
                case ID_NU_LASTDIRECTORY: varDefault = ""; break;
                default: varDefault = "";
            }
        }

        QVariant variant = pSettings->value(sName, varDefault);

        if (!variant.toString().contains("$data")) {
            if ((id == ID_SCAN_DATABASEPATH) || (id == ID_SCAN_CUSTOMDATABASEPATH) || (id == ID_DATAPATH) || (id == ID_SIGNATURES_PATH) || (id == ID_STRUCTS_PATH) ||
                (id == ID_STRUCTSPATH) || (id == ID_INFO_PATH) || (id == ID_SCAN_YARARULESPATH)) {
                if (!QDir(variant.toString()).exists()) {
                    variant = varDefault;
                }
            }
        }

        if (bIsNative) {
            if (variant.toString().contains("$data")) {
                QString sValue = variant.toString();
                sValue = sValue.replace("$data", getApplicationDataPath());
                variant = sValue;
            }
        }

        g_mapValues.insert(id, variant);
    }

    QString sLastDirectory = g_mapValues.value(ID_NU_LASTDIRECTORY).toString();

    if (sLastDirectory != "") {
        if (!QDir(sLastDirectory).exists()) {
            g_mapValues.insert(ID_NU_LASTDIRECTORY, "");
        }
    }

    delete pSettings;
}

void XOptions::save()
{
    QSettings *pSettings = nullptr;

    bool bIsNative = isNative();

    if (bIsNative) {
        pSettings = new QSettings;
    } else {
        pSettings = new QSettings(getApplicationDataPath() + QDir::separator() + QString("%1").arg(g_sName), QSettings::IniFormat);
    }

#ifdef QT_DEBUG
    if (pSettings) {
        qDebug("XOptions save file %s", pSettings->fileName().toUtf8().data());
    }
#endif

    qint32 nNumberOfIDs = g_listValueIDs.count();

    for (qint32 i = 0; i < nNumberOfIDs; i++) {
        ID id = g_listValueIDs.at(i);
        QString sName = idToString(id);
        pSettings->setValue(sName, g_mapValues.value(id));

        if ((id == ID_FILE_SAVELASTDIRECTORY) && (g_mapValues.value(id).toBool() == false)) {
            pSettings->setValue(idToString(ID_NU_LASTDIRECTORY), "");
        } else if ((id == ID_FILE_SAVERECENTFILES) && (g_mapValues.value(id).toBool() == false)) {
            clearRecentFiles();
        }
    }

    delete pSettings;
}

QVariant XOptions::getValue(XOptions::ID id)
{
    return g_mapValues.value(id);
}

void XOptions::setValue(XOptions::ID id, QVariant vValue)
{
    if ((id == ID_VIEW_STYLE) || (id == ID_VIEW_LANG) || (id == ID_VIEW_QSS)) {
        QVariant vOld = g_mapValues.value(id);

        if (vValue != vOld) {
            g_bIsNeedRestart = true;
        }
    }

    g_mapValues.insert(id, vValue);
}

void XOptions::clearValue(XOptions::ID id)
{
    g_mapValues.insert(id, "");
}

QVariant XOptions::getDefaultValue(ID id)
{
    return g_mapDefaultValues.value(id);
}

QString XOptions::idToString(ID id)
{
    QString sResult;

    switch (id) {
        // TODO remove
        case ID_ROOTPATH: sResult = QString("RootPath"); break;
        case ID_DATAPATH: sResult = QString("DataPath"); break;
        case ID_JSON: sResult = QString("Json"); break;
        case ID_STRUCTSPATH: sResult = QString("StructsPath"); break;
        case ID_AUTHUSER: sResult = QString("AuthUser"); break;
        case ID_AUTHTOKEN: sResult = QString("AuthToken"); break;
        // new
        case ID_VIEW_STAYONTOP: sResult = QString("View/StayOnTop"); break;
        case ID_VIEW_STYLE: sResult = QString("View/Style"); break;
        case ID_VIEW_QSS: sResult = QString("View/Qss"); break;
        case ID_VIEW_LANG: sResult = QString("View/Lang"); break;
        case ID_VIEW_SINGLEAPPLICATION: sResult = QString("View/SingleApplication"); break;
        case ID_VIEW_SHOWLOGO: sResult = QString("View/ShowLogo"); break;
        case ID_VIEW_FONT: sResult = QString("View/Font"); break;
        case ID_VIEW_ADVANCED: sResult = QString("View/Advanced"); break;
        case ID_VIEW_SELECTSTYLE: sResult = QString("View/SelectStyle"); break;
        case ID_FILE_SAVELASTDIRECTORY: sResult = QString("File/SaveLastDirectory"); break;
        case ID_FILE_SAVERECENTFILES: sResult = QString("File/SaveRecentFiles"); break;
        case ID_FILE_SAVEBACKUP: sResult = QString("File/SaveBackup"); break;
        case ID_FILE_CONTEXT: sResult = QString("File/Context"); break;
        case ID_SCAN_SCANAFTEROPEN: sResult = QString("Scan/ScanAfterOpen"); break;
        case ID_SCAN_RECURSIVE: sResult = QString("Scan/Recursive"); break;
        case ID_SCAN_DEEP: sResult = QString("Scan/Deep"); break;
        case ID_SCAN_HEURISTIC: sResult = QString("Scan/Heuristic"); break;
        case ID_SCAN_VERBOSE: sResult = QString("Scan/Verbose"); break;
        case ID_SCAN_ALLTYPES: sResult = QString("Scan/AllTypes"); break;
        case ID_SCAN_ENGINE: sResult = QString("Scan/Engine"); break;
        case ID_SCAN_DATABASEPATH: sResult = QString("Scan/DatabasePath"); break;
        case ID_SCAN_CUSTOMDATABASEPATH: sResult = QString("Scan/UserDatabasePath"); break;
        case ID_SCAN_YARARULESPATH: sResult = QString("Scan/YaraRulesPath"); break;
        case ID_SCAN_EDITORFONT: sResult = QString("Scan/EditorFont"); break;
        case ID_SIGNATURES_PATH: sResult = QString("Signatures/Path"); break;
        case ID_INFO_PATH: sResult = QString("Info/Path"); break;
        case ID_ONLINETOOLS_VIRUSTOTAL_APIKEY: sResult = QString("OnlineTools/VirusTotalApi"); break;
        case ID_DISASM_FONT: sResult = QString("Disasm/Font"); break;
        case ID_DISASM_ADDRESSCOLON: sResult = QString("Disasm/AddressColon"); break;
        case ID_DISASM_UPPERCASE: sResult = QString("Disasm/Uppercase"); break;
        case ID_DISASM_HIGHLIGHT: sResult = QString("Disasm/Highlight"); break;
        case ID_DISASM_SYNTAX: sResult = QString("Disasm/Syntax"); break;
        case ID_DISASM_COLOR_ARROWS: sResult = QString("Disasm/Color/Arrows"); break;
        case ID_DISASM_COLOR_ARROWS_SELECTED: sResult = QString("Disasm/Color/Arrows/Selected"); break;
        case ID_DISASM_COLOR_REGS: sResult = QString("Disasm/Color/Regs"); break;
        case ID_DISASM_COLOR_NUMBERS: sResult = QString("Disasm/Color/Numbers"); break;
        case ID_DISASM_COLOR_OPCODE: sResult = QString("Disasm/Color/Opcode"); break;
        case ID_DISASM_COLOR_REFS: sResult = QString("Disasm/Color/Refs"); break;
        case ID_DISASM_COLOR_X86_REGS_GENERAL: sResult = QString("Disasm/Color/x86/Regs/General"); break;
        case ID_DISASM_COLOR_X86_REGS_STACK: sResult = QString("Disasm/Color/x86/Regs/Stack"); break;
        case ID_DISASM_COLOR_X86_REGS_SEGMENT: sResult = QString("Disasm/Color/x86/Regs/Segment"); break;
        case ID_DISASM_COLOR_X86_REGS_DEBUG: sResult = QString("Disasm/Color/x86/Regs/Debug"); break;
        case ID_DISASM_COLOR_X86_REGS_IP: sResult = QString("Disasm/Color/x86/Regs/IP"); break;
        case ID_DISASM_COLOR_X86_REGS_FLAGS: sResult = QString("Disasm/Color/x86/Regs/Flags"); break;
        case ID_DISASM_COLOR_X86_REGS_FPU: sResult = QString("Disasm/Color/x86/Regs/Float"); break;
        case ID_DISASM_COLOR_X86_REGS_XMM: sResult = QString("Disasm/Color/x86/Regs/XMM"); break;
        case ID_DISASM_COLOR_X86_OPCODE_CALL: sResult = QString("Disasm/Color/x86/Opcode/call"); break;
        case ID_DISASM_COLOR_X86_OPCODE_RET: sResult = QString("Disasm/Color/x86/Opcode/ret"); break;
        case ID_DISASM_COLOR_X86_OPCODE_JCC: sResult = QString("Disasm/Color/x86/Opcode/jcc"); break;
        case ID_DISASM_COLOR_X86_OPCODE_PUSH: sResult = QString("Disasm/Color/x86/Opcode/push"); break;
        case ID_DISASM_COLOR_X86_OPCODE_POP: sResult = QString("Disasm/Color/x86/Opcode/pop"); break;
        case ID_DISASM_COLOR_X86_OPCODE_NOP: sResult = QString("Disasm/Color/x86/Opcode/nop"); break;
        case ID_DISASM_COLOR_X86_OPCODE_JMP: sResult = QString("Disasm/Color/x86/Opcode/jmp"); break;
        case ID_DISASM_COLOR_X86_OPCODE_INT3: sResult = QString("Disasm/Color/x86/Opcode/int3"); break;
        case ID_DISASM_COLOR_X86_OPCODE_SYSCALL: sResult = QString("Disasm/Color/x86/Opcode/syscall"); break;
        case ID_DISASM_COLOR_ARM_REGS_GENERAL: sResult = QString("Disasm/Color/arm/Regs/General"); break;
        case ID_DISASM_COLOR_ARM_OPCODE_B: sResult = QString("Disasm/Color/arm/Opcode/b"); break;
        case ID_DISASM_COLOR_ARM_OPCODE_BL: sResult = QString("Disasm/Color/arm/Opcode/bl"); break;
        case ID_DISASM_COLOR_ARM_OPCODE_RET: sResult = QString("Disasm/Color/arm/Opcode/ret"); break;
        case ID_DISASM_COLOR_ARM_OPCODE_PUSH: sResult = QString("Disasm/Color/arm/Opcode/push"); break;
        case ID_DISASM_COLOR_ARM_OPCODE_POP: sResult = QString("Disasm/Color/arm/Opcode/pop"); break;
        case ID_DISASM_COLOR_ARM_OPCODE_NOP: sResult = QString("Disasm/Color/arm/Opcode/nop"); break;
        case ID_HEX_FONT: sResult = QString("Hex/Font"); break;
        case ID_HEX_ADDRESSCOLON: sResult = QString("Hex/AddressColon"); break;
        case ID_STACK_FONT: sResult = QString("Stack/Font"); break;
        case ID_STACK_ADDRESSCOLON: sResult = QString("Stack/AddressColon"); break;
        case ID_REGISTERS_FONT: sResult = QString("Registers/Font"); break;
        case ID_DEBUGGER_COLOR_BREAKPOINT: sResult = QString("Debugger/Color/Breakpoint"); break;
        case ID_IODRIVER_FILENAME: sResult = QString("IODriver/FileName"); break;
        case ID_IODRIVER_SERVICENAME: sResult = QString("IODriver/ServiceName"); break;
        case ID_STRUCTS_PATH: sResult = QString("Structs/Path"); break;
        case ID_NU_RECENTFILES: sResult = QString("RecentFiles"); break;
        case ID_NU_LASTDIRECTORY: sResult = QString("LastDirectory"); break;
        default: sResult = QString("Unknown");
    }

    return sResult;
}

QString XOptions::getLastDirectory()
{
    QString sResult;

    bool bSaveLastDirectory = getValue(ID_FILE_SAVELASTDIRECTORY).toBool();
    QString sLastDirectory = getValue(ID_NU_LASTDIRECTORY).toString();

    if (bSaveLastDirectory && (sLastDirectory != "") && QDir().exists(sLastDirectory)) {
        sResult = sLastDirectory;
    }

    return sResult;
}

void XOptions::setLastDirectory(const QString &sPathName)
{
    QString _sPathName = sPathName;

    if (getValue(ID_FILE_SAVELASTDIRECTORY).toBool()) {
        QFileInfo fi(_sPathName);

        if (fi.isFile()) {
            _sPathName = fi.absolutePath();
        } else if (fi.isDir()) {
            _sPathName = fi.absoluteFilePath();
        }

        setValue(ID_NU_LASTDIRECTORY, _sPathName);
    }
}

void XOptions::setLastFileName(const QString &sFileName)
{
    QFileInfo fi(sFileName);

    QString sDirectory;

    if (fi.isFile()) {
        sDirectory = fi.absolutePath();
    }

    if (getValue(ID_FILE_SAVELASTDIRECTORY).toBool()) {
        setValue(ID_NU_LASTDIRECTORY, sDirectory);
    }

    if (getValue(ID_FILE_SAVERECENTFILES).toBool()) {
        QString _sFileName = fi.absoluteFilePath();

        if (_sFileName != "") {
            QList<QVariant> listFiles = getValue(ID_NU_RECENTFILES).toList();

            listFiles.removeAll(_sFileName);

            listFiles.append(QVariant(_sFileName));

            if (listFiles.count() > g_nMaxRecentFilesCount) {
                listFiles.removeFirst();
            }

            g_mapValues.insert(ID_NU_RECENTFILES, listFiles);

#ifdef QT_GUI_LIB
            _updateRecentFilesMenu();
#endif
        }
    }
}

void XOptions::clearRecentFiles()
{
    g_mapValues.insert(ID_NU_RECENTFILES, QList<QVariant>());

#ifdef QT_GUI_LIB
    _updateRecentFilesMenu();
#endif
}

void XOptions::openRecentFile()
{
#ifdef QT_GUI_LIB
    QAction *pAction = qobject_cast<QAction *>(sender());

    if (pAction) {
        QString sFileName = pAction->data().toString();

        if (QFile::exists(sFileName)) {
            emit openFile(sFileName);
        } else {
            emit errorMessage(QString("%1: %2").arg(tr("Cannot find file"), sFileName));
        }
    }
#endif
}

void XOptions::setCodePageSlot()
{
#ifdef QT_GUI_LIB
    QAction *pAction = qobject_cast<QAction *>(sender());

    if (pAction) {
        QString sCodePage = pAction->data().toString();

        emit setCodePage(sCodePage);
    }
#endif
}

QList<QString> XOptions::getRecentFiles()
{
    QList<QString> listResult;

    QList<QVariant> listFiles = getValue(ID_NU_RECENTFILES).toList();

    qint32 nNumberOfFiles = listFiles.size();

    for (qint32 i = 0; i < nNumberOfFiles; i++) {
        listResult.append(listFiles.at(i).toString());
    }

    return listResult;
}

QString XOptions::getDatabasePath()
{
    return getValue(ID_SCAN_DATABASEPATH).toString();
}

QString XOptions::getCustomDatabasePath()
{
    return getValue(ID_SCAN_CUSTOMDATABASEPATH).toString();
}

QString XOptions::getInfoPath()
{
    return getValue(ID_INFO_PATH).toString();
}

QString XOptions::getScanEngine()
{
    return getValue(ID_SCAN_ENGINE).toString();
}

QString XOptions::getDisasmSyntax()
{
    return getValue(ID_DISASM_SYNTAX).toString();
}

QString XOptions::getRootPath()
{
    return getValue(ID_ROOTPATH).toString();
}

QString XOptions::getDataPath()
{
    return getValue(ID_DATAPATH).toString();
}

QString XOptions::getJson()
{
    return getValue(ID_JSON).toString();
}

QString XOptions::getAuthUser()
{
    return getValue(ID_AUTHUSER).toString();
}

QString XOptions::getAuthToken()
{
    return getValue(ID_AUTHTOKEN).toString();
}

QString XOptions::getVirusTotalApiKey()
{
    return getValue(ID_ONLINETOOLS_VIRUSTOTAL_APIKEY).toString();
}

#ifdef QT_GUI_LIB
void XOptions::adjustStayOnTop(QWidget *pWidget)
{
    _adjustStayOnTop(pWidget, isStayOnTop());
}
#endif
#ifdef QT_GUI_LIB
void XOptions::_adjustStayOnTop(QWidget *pWidget, bool bState)
{
    Qt::WindowFlags wf = pWidget->windowFlags();

    if (bState) {
        wf |= Qt::WindowStaysOnTopHint;
    } else {
        wf &= ~(Qt::WindowStaysOnTopHint);
    }

    pWidget->setWindowFlags(wf);
    pWidget->show();
}
#endif
#ifdef QT_GUI_LIB
void XOptions::_adjustApplicationModal(QWidget *pWidget, bool bState)
{
    if (bState) {
        pWidget->setWindowModality(Qt::ApplicationModal);
    } else {
        pWidget->setWindowModality(Qt::NonModal);
    }

    pWidget->show();
}
#endif
#ifdef QT_GUI_LIB
void XOptions::_adjustFullScreen(QWidget *pWidget, bool bState)
{
    if (bState) {
        pWidget->showFullScreen();
    } else {
        pWidget->showNormal();
    }
}
#endif
#ifdef QT_GUI_LIB
void XOptions::adjustFont(QWidget *pWidget)
{
    QFont _font;
    QString sFont = getValue(XOptions::ID_VIEW_FONT).toString();

    if ((sFont != "") && _font.fromString(sFont)) {
        pWidget->setFont(_font);
    }
}
#endif
#ifdef QT_GUI_LIB
void XOptions::adjustWindow(QWidget *pWidget)
{
    if (isIDPresent(XOptions::ID_VIEW_STAYONTOP)) {
        adjustStayOnTop(pWidget);
    }

    if (isIDPresent(XOptions::ID_VIEW_FONT)) {
        adjustFont(pWidget);
    }
}
#endif
#ifdef QT_GUI_LIB
void XOptions::setMonoFont(QWidget *pWidget, qint32 nSize)
{
    QFont font = pWidget->font();

#ifdef Q_OS_WIN
    font.setFamily("Courier");
#endif
#ifdef Q_OS_LINUX
    font.setFamily("DejaVu Sans Mono");
#endif
#ifdef Q_OS_MACOS
    font.setFamily("Menlo");  // TODO Check
#endif

    if (nSize != -1) {
        font.setPointSize(nSize);
    }

    pWidget->setFont(font);
}
#endif
#ifdef QT_GUI_LIB
void XOptions::setCheckBox(QCheckBox *pCheckBox, XOptions::ID id)
{
    pCheckBox->setChecked(getValue(id).toBool());
}
#endif
#ifdef QT_GUI_LIB
void XOptions::getCheckBox(QCheckBox *pCheckBox, XOptions::ID id)
{
    setValue(id, pCheckBox->isChecked());
}
#endif
#ifdef QT_GUI_LIB
void XOptions::setCheckBox(QGroupBox *pGroupBox, ID id)
{
    pGroupBox->setChecked(getValue(id).toBool());
}
#endif
#ifdef QT_GUI_LIB
void XOptions::getCheckBox(QGroupBox *pGroupBox, ID id)
{
    setValue(id, pGroupBox->isChecked());
}
#endif
#ifdef QT_GUI_LIB
void XOptions::setComboBox(QComboBox *pComboBox, XOptions::ID id)
{
    const bool bBlocked1 = pComboBox->blockSignals(true);

    pComboBox->clear();

    QString sValue = getValue(id).toString();

    if (id == ID_VIEW_STYLE) {
        pComboBox->addItem("Default", "");
        QStringList listKeys = QStyleFactory::keys();

        qint32 nNumberOfKeys = listKeys.count();

        for (qint32 i = 0; i < nNumberOfKeys; i++) {
            QString sRecord = listKeys.at(i);
            pComboBox->addItem(sRecord, sRecord);
        }
    } else if (id == ID_VIEW_LANG) {
        pComboBox->addItem("English", "");
        pComboBox->addItem("System", "System");

        QList<QString> listFileNames = getAllFilesFromDirectory(getApplicationLangPath(), "*.qm");

        qint32 nNumberOfRecords = listFileNames.count();

        for (qint32 i = 0; i < nNumberOfRecords; i++) {
            QFileInfo fi(listFileNames.at(i));

            QString sRecord = fi.completeBaseName();

            QLocale locale(sRecord.section("_", 1, -1));
            QString sLocale = locale.nativeLanguageName();

            if (sRecord.count("_") == 2) {
                sLocale += QString("(%1)").arg(locale.nativeCountryName());
            }

            if (sLocale != "") {
                sLocale.replace(0, 1, sLocale[0].toUpper());
            }

            sLocale += QString("[%1]").arg(locale.languageToString(locale.language()));

            pComboBox->addItem(sLocale, sRecord);
        }
    } else if (id == ID_VIEW_QSS) {
        pComboBox->addItem("Default", "");

        QList<QString> listFileNames = getAllFilesFromDirectory(getApplicationQssPath(), "*.qss");

        qint32 nNumberOfRecords = listFileNames.count();

        for (qint32 i = 0; i < nNumberOfRecords; i++) {
            QFileInfo fi(listFileNames.at(i));

            QString sRecord = fi.completeBaseName();

            pComboBox->addItem(sRecord, sRecord);
        }
    } else if (id == ID_SCAN_ENGINE) {
        pComboBox->addItem(tr("Automatic"), "auto");
        pComboBox->addItem(QString("Detect It Easy (DiE)"), "die");
        pComboBox->addItem(QString("Nauz File Detector (NFD)"), "nfd");
        pComboBox->addItem(QString("Yara rules"), "yara");
    } else if (id == ID_DISASM_SYNTAX) {
        pComboBox->addItem("Default", "");
        pComboBox->addItem(QString("ATT"), "ATT");
        pComboBox->addItem(QString("INTEL"), "INTEL");
        pComboBox->addItem(QString("MASM"), "MASM");
        pComboBox->addItem(QString("MOTOROLA"), "MOTOROLA");
    }

    qint32 nNumberOfItems = pComboBox->count();
    qint32 nIndex = -1;

    for (qint32 i = 0; i < nNumberOfItems; i++) {
        if (sValue == pComboBox->itemData(i, Qt::UserRole)) {
            nIndex = i;
        }
    }

    if (nIndex != -1) {
        pComboBox->setCurrentIndex(nIndex);
    }

    pComboBox->blockSignals(bBlocked1);
}
#endif
#ifdef QT_GUI_LIB
void XOptions::getComboBox(QComboBox *pComboBox, XOptions::ID id)
{
    setValue(id, pComboBox->currentData());
}
#endif
#ifdef QT_GUI_LIB
void XOptions::setLineEdit(QLineEdit *pLineEdit, XOptions::ID id)
{
    pLineEdit->setText(getValue(id).toString());
}
#endif
#ifdef QT_GUI_LIB
void XOptions::getLineEdit(QLineEdit *pLineEdit, XOptions::ID id)
{
    setValue(id, pLineEdit->text());
}
#endif
bool XOptions::isSaveBackup()
{
    return getValue(XOptions::ID_FILE_SAVEBACKUP).toBool();
}

bool XOptions::isSaveLastDirectory()
{
    return getValue(XOptions::ID_FILE_SAVELASTDIRECTORY).toBool();
}

bool XOptions::isSaveRecentFiles()
{
    return getValue(XOptions::ID_FILE_SAVERECENTFILES).toBool();
}

bool XOptions::isRestartNeeded()
{
    return g_bIsNeedRestart;
}

bool XOptions::isStayOnTop()
{
    return getValue(XOptions::ID_VIEW_STAYONTOP).toBool();
}

bool XOptions::isScanAfterOpen()
{
    return getValue(XOptions::ID_SCAN_SCANAFTEROPEN).toBool();
}

bool XOptions::isRecursiveScan()
{
    return getValue(XOptions::ID_SCAN_RECURSIVE).toBool();
}

bool XOptions::isDeepScan()
{
    return getValue(XOptions::ID_SCAN_DEEP).toBool();
}

bool XOptions::isHeuristicScan()
{
    return getValue(XOptions::ID_SCAN_HEURISTIC).toBool();
}

bool XOptions::isVerboseScan()
{
    return getValue(XOptions::ID_SCAN_VERBOSE).toBool();
}

bool XOptions::isAllTypesScan()
{
    return getValue(XOptions::ID_SCAN_ALLTYPES).toBool();
}

bool XOptions::isSingleApplication()
{
    return getValue(XOptions::ID_VIEW_SINGLEAPPLICATION).toBool();
}

bool XOptions::isShowLogo()
{
    return getValue(XOptions::ID_VIEW_SHOWLOGO).toBool();
}

QString XOptions::getSearchSignaturesPath()
{
    return getValue(XOptions::ID_SIGNATURES_PATH).toString();
}

QString XOptions::getStructsPath()
{
    return getValue(XOptions::ID_STRUCTS_PATH).toString();
}
#ifdef QT_GUI_LIB
void XOptions::adjustApplicationView(const QString &sTranslationName, XOptions *pOptions)
{
    if (pOptions->isIDPresent(XOptions::ID_VIEW_STYLE)) {
        QString sStyle = pOptions->getValue(XOptions::ID_VIEW_STYLE).toString();

        if (sStyle != "") {
            qApp->setStyle(QStyleFactory::create(sStyle));
        }
    }

    if (pOptions->isIDPresent(XOptions::ID_VIEW_LANG)) {
        QTranslator *pTranslator = new QTranslator;  // Important
        QString sLang = pOptions->getValue(XOptions::ID_VIEW_LANG).toString();
        QString sLangsPath = pOptions->getApplicationLangPath();

        bool bLoad = false;

        if (sLang == "System") {
            QLocale locale = QLocale::system();
            if (locale != QLocale::English) {
                bLoad = pTranslator->load(locale, sTranslationName, "_", sLangsPath, ".qm");
            }
        } else if (sLang != "") {
            bLoad = pTranslator->load(sLang, sLangsPath);
        }

        if (bLoad) {
            qApp->installTranslator(pTranslator);
        }
    }

    if (pOptions->isIDPresent(XOptions::ID_VIEW_QSS)) {
        QString sQss = pOptions->getValue(XOptions::ID_VIEW_QSS).toString();

        if (sQss != "") {
            QString sQssFileName = pOptions->getApplicationQssPath() + QDir::separator() + QString("%1.qss").arg(sQss);

            if (QFile::exists(sQssFileName)) {
                QFile file;
                file.setFileName(sQssFileName);

                if (file.open(QIODevice::ReadOnly)) {
                    QByteArray baQss = file.readAll();
                    qApp->setStyleSheet(baQss.data());
                    file.close();
                }
            }
        }
    }

    if (pOptions->isIDPresent(XOptions::ID_VIEW_FONT)) {
        QString sFont = pOptions->getValue(XOptions::ID_VIEW_FONT).toString();

        if (sFont != "") {
            qApp->setFont(sFont);
        }
    }
}
#endif
#ifdef QT_GUI_LIB
void XOptions::adjustListWidget(QListWidget *pListWidget, qint32 nMinimumWidth)
{
    qint32 nWidth = nMinimumWidth;

    qint32 nNumberOfRecords = pListWidget->count();

    for (qint32 i = 0; i < nNumberOfRecords; i++) {
        QString sText = pListWidget->item(i)->text() + "WWWW";
        nWidth = qMax(QFontMetrics(pListWidget->item(i)->font()).size(Qt::TextSingleLine, sText).width(), nWidth);
    }

    pListWidget->setMinimumWidth(0);
    pListWidget->setMaximumWidth(nWidth);
}
#endif
// #ifdef QT_GUI_LIB
//  void XOptions::adjustApplicationView(QString sApplicationFileName,QString
//  sTranslationName)
//{
//     XOptions xOptions;

//    xOptions.setName(sApplicationFileName);

//    QList<XOptions::ID> listIDs;

//    listIDs.append(XOptions::ID_VIEW_STYLE);
//    listIDs.append(XOptions::ID_VIEW_LANG);
//    listIDs.append(XOptions::ID_VIEW_QSS);

//    xOptions.setValueIDs(listIDs);
//    xOptions.load();

//    xOptions.adjustApplicationView(sTranslationName,&xOptions);
//}
// #endif
#ifdef QT_GUI_LIB
QWidget *XOptions::getMainWidget(QWidget *pWidget)
{
    QWidget *pResult = pWidget;

    if (pResult) {
        while (pResult->parent()) {
            pResult = qobject_cast<QWidget *>(pResult->parent());
        }
    }

    return pResult;
}
#endif
#ifdef QT_GUI_LIB
QString XOptions::getTableModelText(QAbstractItemModel *pModel)
{
    QString sResult;

    if (pModel) {
        qint32 nNumberOfRows = pModel->rowCount();
        qint32 nNumberOfColumns = pModel->columnCount();

        QList<QString> listHeaders;
        QList<QList<QString>> listListStrings;

        for (qint32 i = 0; i < nNumberOfColumns; i++) {
            QString sHeader = pModel->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString();

            listHeaders.append(sHeader);
        }

        for (qint32 i = 0; i < nNumberOfRows; i++) {
            QList<QString> listStrings;

            for (qint32 j = 0; j < nNumberOfColumns; j++) {
                QString sString = pModel->data(pModel->index(i, j)).toString();

                listStrings.append(sString);
            }

            listListStrings.append(listStrings);
        }

        for (qint32 i = 0; i < nNumberOfColumns; i++) {
            if (i != (nNumberOfColumns - 1)) {
                sResult += QString("%1\t").arg(listHeaders.at(i));
            } else {
                sResult += QString("%1\r\n").arg(listHeaders.at(i));
            }
        }

        // mb TODO csv,tsv,json,xml,json
        qint32 _nNumberOfLines = listListStrings.count();

        for (qint32 i = 0; i < _nNumberOfLines; i++) {
            qint32 _nNumberOfColumns = listListStrings.at(i).count();

            for (qint32 j = 0; j < _nNumberOfColumns; j++) {
                QString sString = listListStrings.at(i).at(j);

                if (j != (_nNumberOfColumns - 1)) {
                    sResult += QString("%1\t").arg(sString);
                } else {
                    sResult += QString("%1\r\n").arg(sString);
                }
            }
        }
    }

    return sResult;
}
#endif
#ifdef QT_GUI_LIB
void XOptions::_getTreeModelText(QString *psString, QAbstractItemModel *pModel, QModelIndex index, qint32 nLevel)
{
    qint32 nNumberOfRows = pModel->rowCount(index);
    qint32 nNumberOfColumns = pModel->columnCount(index);

    for (qint32 i = 0; i < nNumberOfRows; i++) {
        QString sPrefix;
        sPrefix = sPrefix.leftJustified(4 * nLevel, ' ');

        *psString += sPrefix;

        for (qint32 j = 0; j < nNumberOfColumns; j++) {
            QString sString = pModel->data(pModel->index(i, j, index)).toString();

            if (j != (nNumberOfColumns - 1)) {
                *psString += QString("%1\t").arg(sString);
            } else {
                *psString += QString("%1\r\n").arg(sString);
            }
        }

        if (nNumberOfColumns) {
            _getTreeModelText(psString, pModel, pModel->index(i, 0, index), nLevel + 1);
        }
    }
}
#endif
#ifdef QT_GUI_LIB
QString XOptions::getTreeModelText(QAbstractItemModel *pModel)
{
    QString sResult;

    if (pModel) {
        _getTreeModelText(&sResult, pModel, QModelIndex(), 0);
    }

    return sResult;
}
#endif
#ifdef QT_GUI_LIB
bool XOptions::saveTableModel(QAbstractItemModel *pModel, const QString &sFileName)
{
    bool bResult = false;

    QFile file;
    file.setFileName(sFileName);

    if (file.open(QIODevice::ReadWrite)) {
        QString sText = getTableModelText(pModel);

        file.resize(0);
        file.write(sText.toUtf8().data());
        file.close();

        bResult = true;
    }

    return bResult;
}
#endif
#ifdef QT_GUI_LIB
bool XOptions::saveTreeModel(QAbstractItemModel *pModel, const QString &sFileName)
{
    bool bResult = false;

    QFile file;
    file.setFileName(sFileName);

    if (file.open(QIODevice::ReadWrite)) {
        QString sText = getTreeModelText(pModel);

        file.resize(0);
        file.write(sText.toUtf8().data());
        file.close();

        bResult = true;
    }

    return bResult;
}
#endif
#ifdef QT_GUI_LIB
bool XOptions::saveTextEdit(QTextEdit *pTextEdit, const QString &sFileName)
{
    bool bResult = false;

    QFile file;
    file.setFileName(sFileName);

    if (file.open(QIODevice::ReadWrite)) {
        QString sResult = pTextEdit->toPlainText();

        file.resize(0);
        file.write(sResult.toUtf8().data());
        file.close();

        bResult = true;
    }

    return bResult;
}
#endif
#ifdef QT_GUI_LIB
bool XOptions::savePlainTextEdit(QPlainTextEdit *pPlainTextEdit, const QString &sFileName)
{
    bool bResult = false;

    QFile file;
    file.setFileName(sFileName);

    if (file.open(QIODevice::ReadWrite)) {
        QString sResult = pPlainTextEdit->toPlainText();

        file.resize(0);
        file.write(sResult.toUtf8().data());
        file.close();

        bResult = true;
    }

    return bResult;
}
#endif
#ifdef QT_GUI_LIB
bool XOptions::saveTableView(QTableView *pTableView, const QString &sFileName)
{
    return saveTableModel(pTableView->model(), sFileName);
}
#endif
#ifdef QT_GUI_LIB
bool XOptions::saveTableWidget(QTableWidget *pTableWidget, const QString &sFileName)
{
    return saveTableModel(pTableWidget->model(), sFileName);
}
#endif
#ifdef QT_GUI_LIB
bool XOptions::saveListView(QListView *pListView, const QString &sFileName)
{
    return saveTableModel(pListView->model(), sFileName);
}
#endif
#ifdef QT_GUI_LIB
bool XOptions::saveListWidget(QListWidget *pListWidget, const QString &sFileName)
{
    return saveTableModel(pListWidget->model(), sFileName);
}
#endif
#ifdef QT_GUI_LIB
bool XOptions::saveTreeView(QTreeView *pTreeView, const QString &sFileName)
{
    return saveTreeModel(pTreeView->model(), sFileName);
}
#endif
#ifdef QT_GUI_LIB
bool XOptions::saveTreeWidget(QTreeWidget *pTreeWidget, const QString &sFileName)
{
    return saveTreeModel(pTreeWidget->model(), sFileName);
}
#endif
#ifdef QT_GUI_LIB
bool XOptions::saveTextBrowser(QTextBrowser *pTextBrowser, const QString &sFileName)
{
    bool bResult = false;

    QFile file;
    file.setFileName(sFileName);

    if (file.open(QIODevice::ReadWrite)) {
        QString sResult = pTextBrowser->toPlainText();

        file.resize(0);
        file.write(sResult.toUtf8().data());
        file.close();

        bResult = true;
    }

    return bResult;
}
#endif
#ifdef QT_GUI_LIB
bool XOptions::saveTextBrowserHtml(QTextBrowser *pTextBrowser, const QString &sFileName)
{
    bool bResult = false;

    QFile file;
    file.setFileName(sFileName);

    if (file.open(QIODevice::ReadWrite)) {
        QString sResult = pTextBrowser->toHtml();

        file.resize(0);
        file.write(sResult.toUtf8().data());
        file.close();

        bResult = true;
    }

    return bResult;
}
#endif
#ifdef QT_GUI_LIB
QMenu *XOptions::createRecentFilesMenu(QWidget *pParent)
{
    g_pRecentFilesMenu = new QMenu(tr("Recent files"), pParent);

    _updateRecentFilesMenu();

    return g_pRecentFilesMenu;
}
#endif
#ifdef QT_GUI_LIB
qint32 XOptions::getCharWidth(QWidget *pWidget)
{
    const QFontMetricsF fm(pWidget->font());

    return fm.maxWidth();
}
#endif
#ifdef QT_GUI_LIB
qint32 XOptions::getCharHeight(QWidget *pWidget)
{
    const QFontMetricsF fm(pWidget->font());

    return fm.height();
}
#endif
#ifdef QT_GUI_LIB
void XOptions::showInFolder(const QString &sFileName)
{
    // TODO https://github.com/qt-creator/qt-creator/blob/master/src/plugins/coreplugin/fileutils.cpp#L67
    QFileInfo fi = QFileInfo(sFileName);

#if defined(Q_OS_WIN)
    QStringList slParams;
    if (!fi.isDir()) {
        slParams += QLatin1String("/select,");
    }

    slParams += QDir::toNativeSeparators(fi.canonicalFilePath());

    QProcess::startDetached("explorer.exe", slParams);
#elif defined(Q_OS_MAC)
    QStringList slParams;
    slParams << "-e";
    slParams << "tell application \"Finder\"";
    slParams << "-e";
    slParams << "activate";
    slParams << "-e";
    slParams << "select POSIX file \"" + fi.path() + "\"";
    slParams << "-e";
    slParams << "end tell";
    slParams << "-e";
    slParams << "return";
    QProcess::execute("/usr/bin/osascript", slParams);
#else
    QDesktopServices::openUrl(QUrl::fromLocalFile(fi.path()));
#endif
}
#endif
#ifdef QT_GUI_LIB
void XOptions::showFolder(const QString &sDirectory)
{
#if defined(Q_OS_WIN)
    QStringList slParams;

    slParams += QDir::toNativeSeparators(sDirectory);

    QProcess::startDetached("explorer.exe", slParams);
#elif defined(Q_OS_MAC)
    QStringList slParams;
    slParams << "-e";
    slParams << "tell application \"Finder\"";
    slParams << "-e";
    slParams << "activate";
    slParams << "-e";
    slParams << "select POSIX file \"" + sDirectory + "\"";  // TODO Check
    slParams << "-e";
    slParams << "end tell";
    slParams << "-e";
    slParams << "return";
    QProcess::execute("/usr/bin/osascript", slParams);
#else
    QDesktopServices::openUrl(QUrl::fromLocalFile(sDirectory));
#endif
}
#endif
#ifdef QT_GUI_LIB
void XOptions::handleFontButton(QWidget *pParent, QLineEdit *pLineEdit)
{
    QFont _font;
    _font.fromString(pLineEdit->text());

    bool bOK = false;
    _font = QFontDialog::getFont(&bOK, _font, pParent);

    if (bOK) {
        pLineEdit->setText(_font.toString());
    }
}
#endif
#ifdef QT_GUI_LIB
void XOptions::setModelTextAlignment(QStandardItemModel *pModel, qint32 nColumn, Qt::Alignment flag)
{
    qint32 nNumberOfRows = pModel->rowCount();

    pModel->setHeaderData(nColumn, Qt::Horizontal, (qint32)flag, Qt::TextAlignmentRole);

    for (qint32 i = 0; i < nNumberOfRows; i++) {
        QStandardItem *pItem = pModel->item(i, nColumn);

        if (pItem) {
            pItem->setTextAlignment(flag);
            // pModel->setData(pModel->index(i, nColumn), (qint32)flag, Qt::TextAlignmentRole);

            QModelIndex index = pModel->index(i, 0);
            qint32 _nNumberOfRows = pModel->rowCount(index);

            for (qint32 j = 0; j < _nNumberOfRows; j++) {
                pModel->setData(pModel->index(j, nColumn, index), (qint32)flag, Qt::TextAlignmentRole);
            }
        }
    }
}
#endif
#ifdef QT_GUI_LIB
void XOptions::setTableViewHeaderWidth(QTableView *pTableView, qint32 nColumn, qint32 nContentWidth)
{
    QFont font = pTableView->font();
    font.setBold(true);

    const QFontMetricsF fm(font);

    QString sTitle = pTableView->model()->headerData(nColumn, Qt::Horizontal, Qt::DisplayRole).toString();

    qreal rWidth = fm.boundingRect(sTitle + "  ").width();

    nContentWidth = qMax(nContentWidth, (qint32)(rWidth));

    pTableView->setColumnWidth(nColumn, nContentWidth);
}
#endif
#ifdef QT_GUI_LIB
void XOptions::setTreeViewHeaderWidth(QTreeView *pTreeView, qint32 nColumn, qint32 nContentWidth)
{
    QFont font = pTreeView->font();
    font.setBold(true);

    const QFontMetricsF fm(font);

    QString sTitle = pTreeView->model()->headerData(nColumn, Qt::Horizontal, Qt::DisplayRole).toString();

    qreal rWidth = fm.boundingRect(sTitle + "  ").width();

    nContentWidth = qMax(nContentWidth, (qint32)(rWidth));

    pTreeView->setColumnWidth(nColumn, nContentWidth);
}
#endif
#ifdef QT_GUI_LIB
void XOptions::setTableWidgetHeaderAlignment(QTableWidget *pTableWidget, qint32 nColumn, Qt::Alignment flag)
{
    pTableWidget->model()->setHeaderData(nColumn, Qt::Horizontal, (qint32)flag, Qt::TextAlignmentRole);
}
#endif
QString XOptions::getApplicationLangPath()
{
    QString sResult;

    sResult = getApplicationDataPath() + QDir::separator() + "lang";

    return sResult;
}

QString XOptions::getApplicationQssPath()
{
    QString sResult;

    sResult = getApplicationDataPath() + QDir::separator() + "qss";

    return sResult;
}

QList<QString> XOptions::getAllFilesFromDirectory(const QString &sDirectory, const QString &sExtension)
{
    QDir directory(sDirectory);

    return directory.entryList(QStringList() << sExtension, QDir::Files);
}

bool XOptions::checkNative(const QString &sIniFileName)
{
    Q_UNUSED(sIniFileName)

    bool bResult = false;
#if defined(Q_OS_MAC)
    bResult = true;
#elif defined(Q_OS_LINUX) || defined(Q_OS_FREEBSD)
    QString sApplicationDirPath = qApp->applicationDirPath();

    if ((sApplicationDirPath == "/bin") || (sApplicationDirPath == "/usr/bin") || (sApplicationDirPath == "/usr/local/bin") ||
        (sApplicationDirPath == "/app/bin") || (sApplicationDirPath.contains("/usr/local/bin$")) || isAppImage()) {
        bResult = true;
    } else {
        bResult = false;
    }
#elif defined(Q_OS_WIN)
    QString sApplicationDirPath = qApp->applicationDirPath();

    if (sApplicationDirPath.contains("C:\\Program Files\\") || sApplicationDirPath.contains("C:\\Program Files (x86)\\")) {
        bResult = true;
    }
#endif

    //    if (!bResult) {
    //        QSettings settings(qApp->applicationDirPath() + QDir::separator() + QString("%1").arg(sIniFileName), QSettings::IniFormat);

    //        bResult = !(settings.isWritable());
    //    }

    return bResult;
}

QString XOptions::getApplicationDataPath()
{
    QString sResult;

    QString sApplicationDirPath = qApp->applicationDirPath();

#ifdef Q_OS_MAC
    sResult = sApplicationDirPath + "/../Resources";
#elif defined(Q_OS_LINUX)
    if (isNative()) {
        if (sApplicationDirPath.contains("/usr/local/bin$")) {
            QString sPrefix = sApplicationDirPath.section("/usr/local/bin", 0, 0);

            sResult += sPrefix + QString("/usr/local/lib/%1").arg(qApp->applicationName());
        } else {
            if (sApplicationDirPath.contains("/tmp/.mount_"))  // AppImage
            {
                sResult = sApplicationDirPath.section("/", 0, 2);
            }

            sResult += QString("/usr/lib/%1").arg(qApp->applicationName());
        }
    } else {
        sResult = sApplicationDirPath;
    }
#elif defined(Q_OS_FREEBSD)
    sResult = QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation).at(1) + QDir::separator() + qApp->applicationName();
#else
    sResult = sApplicationDirPath;
#endif

    return sResult;
}

QString XOptions::getTitle(const QString &sName, const QString &sVersion, bool bShowOS)
{
    QString sResult = QString("%1 v%2").arg(sName, sVersion);

    if (bShowOS) {
#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
        // TODO Check Windows 11
        sResult += QString(" [%3] (%4)").arg(QSysInfo::prettyProductName(), QSysInfo::buildCpuArchitecture());
#else
        // TODO OS Name // For Windows Arch GetVersionExA
#endif
    }

    return sResult;
}

bool XOptions::isWritable()
{
    bool bResult = false;
    QSettings *pSettings = nullptr;

    bool bIsNative = isNative();

    if (bIsNative) {
        pSettings = new QSettings;
    } else {
        pSettings = new QSettings(getApplicationDataPath() + QDir::separator() + QString("%1").arg(g_sName), QSettings::IniFormat);
    }

    bResult = pSettings->isWritable();

    delete pSettings;

    return bResult;
}

#if (QT_VERSION_MAJOR < 6) || defined(QT_CORE5COMPAT_LIB)

bool sort_code_page(const qint32 &nValue1, const qint32 &nValue2)
{
    return (qAbs(nValue1) < qAbs(nValue2));
}

QList<QString> XOptions::getCodePages(bool bAll)
{
    QList<QString> listResult;

    listResult.append("");  // Empty

    QList<qint32> list = QTextCodec::availableMibs();

    std::sort(list.begin(), list.end(), sort_code_page);

    qint32 nNumberOfRecords = list.count();

    for (qint32 i = 0; i < nNumberOfRecords; i++) {
        qint32 nMIB = list.at(i);

        //        qDebug("%s",QTextCodec::codecForMib(nMIB)->name().data());

        bool bAdd = true;

        if (!bAll) {
            if (nMIB == 106) bAdd = false;                            // UTF8
            else if ((nMIB >= 1013) && (nMIB <= 1019)) bAdd = false;  // Unicode
        }

        if (bAdd) {
            QString sName = QTextCodec::codecForMib(nMIB)->name();

            listResult.append(sName);
        }
    }

    return listResult;
}
#endif
void XOptions::registerCodecs()
{
    #if (QT_VERSION_MAJOR < 6) || defined(QT_CORE5COMPAT_LIB)
    {
        codec_cp437 *pCodec = new codec_cp437;

        if (!pCodec) {
            qFatal("Codec failed");
        }
    }
    #endif
}

#if (QT_VERSION_MAJOR < 6) || defined(QT_CORE5COMPAT_LIB)
#ifdef QT_GUI_LIB
QMenu *XOptions::createCodePagesMenu(QWidget *pParent, bool bAll)
{
    g_pCodePagesMenu = new QMenu(tr("Code pages"), pParent);

    if (g_pCodePagesMenu) {
        g_pCodePagesMenu->clear();  // TODO Check

        QList<QString> listCodePages = getCodePages(bAll);

        qint32 nNumberOfRecords = listCodePages.count();

        for (qint32 i = 0; i < nNumberOfRecords; i++) {
            QAction *pAction = new QAction(listCodePages.at(i), g_pCodePagesMenu);
            pAction->setData(listCodePages.at(i));

            connect(pAction, SIGNAL(triggered()), this, SLOT(setCodePageSlot()));

            g_pCodePagesMenu->addAction(pAction);
        }
    }

    return g_pCodePagesMenu;
}
#endif
#endif
#ifdef Q_OS_WIN
QString XOptions::getClassesPrefix(USERROLE userRole)
{
    QString sResult;

    if (userRole == USERROLE_ADMIN) {
        sResult = "HKEY_CLASSES_ROOT";
    } else if (userRole == USERROLE_NORMAL) {
        sResult = "HKEY_CURRENT_USER\\Software\\Classes";
    }

    return sResult;
}
#endif
#ifdef Q_OS_WIN
bool XOptions::registerContext(const QString &sApplicationName, const QString &sType, const QString &sApplicationFilePath, USERROLE userRole)
{
    QString _sApplicationFilePath = sApplicationFilePath;

    QSettings settings(getClassesPrefix(userRole) + QString("\\%1\\shell\\%2\\command").arg(sType, sApplicationName), QSettings::NativeFormat);
    settings.setValue(".", "\"" + _sApplicationFilePath.replace("/", "\\") + "\" \"%1\"");

    QSettings settingsIcon(getClassesPrefix(userRole) + QString("\\%1\\shell\\%2").arg(sType, sApplicationName), QSettings::NativeFormat);
    settingsIcon.setValue("Icon", "\"" + _sApplicationFilePath.replace("/", "\\") + "\"");

    // TODO Check if not send message
    return checkContext(sApplicationName, sType);
}
#endif
#ifdef Q_OS_WIN
bool XOptions::clearContext(const QString &sApplicationName, const QString &sType, USERROLE userRole)
{
    QSettings settings(getClassesPrefix(userRole) + QString("\\%1\\shell\\%2").arg(sType, sApplicationName), QSettings::NativeFormat);
    settings.clear();

    // TODO Check if not send message
    return !(checkContext(sApplicationName, sType));
}
#endif
#ifdef Q_OS_WIN
bool XOptions::checkContext(const QString &sApplicationName, const QString &sType, USERROLE userRole)
{
    QSettings settings(getClassesPrefix(userRole) + QString("\\%1\\shell").arg(sType), QSettings::NativeFormat);

    return (settings.value(QString("%1/command/Default").arg(sApplicationName)).toString() != "");
}
#endif

void XOptions::setMaxRecentFilesCount(qint32 nValue)
{
    g_nMaxRecentFilesCount = nValue;
}

qint32 XOptions::getMaxRecentFilesCount()
{
    return g_nMaxRecentFilesCount;
}

#ifdef QT_GUI_LIB
void XOptions::_updateRecentFilesMenu()
{
    if (g_pRecentFilesMenu) {
        g_pRecentFilesMenu->clear();

        QList<QString> listRecentFiles = getRecentFiles();

        qint32 nNumberOfRecentFiles = listRecentFiles.count();

        for (qint32 i = nNumberOfRecentFiles - 1; i >= 0; i--) {
            QAction *pAction = new QAction(listRecentFiles.at(i), g_pRecentFilesMenu);
            pAction->setData(listRecentFiles.at(i));

            connect(pAction, SIGNAL(triggered()), this, SLOT(openRecentFile()));

            g_pRecentFilesMenu->addAction(pAction);
        }

        if (nNumberOfRecentFiles) {
            g_pRecentFilesMenu->addSeparator();

            QAction *pAction = new QAction(tr("Clear"), g_pRecentFilesMenu);

            connect(pAction, SIGNAL(triggered()), this, SLOT(clearRecentFiles()));

            g_pRecentFilesMenu->addAction(pAction);
        }

        g_pRecentFilesMenu->setEnabled(nNumberOfRecentFiles);
    }
}
#endif
#ifdef QT_GUI_LIB
QString XOptions::getImageFilter()
{
    const QList<QByteArray> listImageFormats = QImageWriter::supportedImageFormats();

    QStringList listFilter;

    qint32 nNumberOfImageFormats = listImageFormats.count();

    if (nNumberOfImageFormats) {
        QString sImageFilter = tr("Images") + " (";

        for (qint32 i = 0; i < nNumberOfImageFormats; i++) {
            if (i > 0) {
                sImageFilter += " ";
            }

            sImageFilter += "*.";
            sImageFilter += listImageFormats.at(i);
        }

        sImageFilter += ")";

        listFilter.append(sImageFilter);
    }

    listFilter.append(QString("PDF %1 (*.pdf)").arg(tr("Documents")));
    //    listFilter.append(QString("Postscript %1 (*.ps)").arg(tr("Documents")));

    return listFilter.join(";;");
}
#endif
#ifdef QT_GUI_LIB
QColor XOptions::getColorDialog(QWidget *pParent, QString sTitle, QColor &color)
{
    QColor colResult;

    QColorDialog colorDialog(pParent);
    colorDialog.setCurrentColor(color);
    colorDialog.setWindowTitle(sTitle);

    _adjustStayOnTop(&colorDialog, true);

    if (colorDialog.exec() == QDialog::Accepted) {
        colResult = colorDialog.currentColor();
    }

    return colResult;
}
#endif
XOptions::BUNDLE XOptions::getBundle()
{
    BUNDLE result = BUNDLE_UNKNOWN;

#ifdef QT_DEBUG
#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
    QString _sProductType = QSysInfo::productType();
    qDebug("OS: %s", _sProductType.toUtf8().data());
#endif
#endif

#ifdef Q_OS_WIN
#if QT_VERSION <= QT_VERSION_CHECK(5, 6, 3)
    result = BUNDLE_WINDOWS_XP_X86;
#elif (QT_VERSION_MAJOR >= 6)
    // TODO ARM
    result = BUNDLE_WINDOWS_QT6_X64;
#else
#ifndef Q_OS_WIN64
    result = BUNDLE_WINDOWS_X86;
#else
    result = BUNDLE_WINDOWS_X64;
#endif
#endif
#endif

#ifdef Q_PROCESSOR_X86_64
#ifdef Q_OS_LINUX
#if QT_VERSION == QT_VERSION_CHECK(5, 2, 1)
    result = BUNDLE_LINUX_APPIMAGE_X64;
#elif QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
    QString sProductType = QSysInfo::productType();

    if (sProductType == "ubuntu") {
        result = BUNDLE_LINUX_UBUNTU_X64;
    }
#endif
#endif
#ifdef Q_OS_MACOS
    // TODO
    // TODO QSysInfo::currentCpyArchitecture();
    // M
#endif
#endif
#ifdef Q_OS_FRREBSD
#ifdef Q_PROCESSOR_X86_64
    result = BUNDLE_FREEBSD_X64;
#endif
#endif
    return result;
}
