import QtQuick 2.4
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import QtQuick.Window 2.3
import QtGraphicalEffects 1.0
import FishUI 1.0 as FishUI
import "../"
FishUI.Window {
    id: rootWindow
    title: qsTr("Cherryimg")
    visible: true
    width: 600
    height: 500

    minimumWidth: 600
    minimumHeight: 500

    header.height: 40
    contentTopMargin: 0

    LayoutMirroring.enabled: Qt.application.layoutDirection === Qt.RightToLeft
    LayoutMirroring.childrenInherit: true


    MainWindow
    {
        id: mainWindow
        anchors.fill: parent
    }

    onClosing: function(closeevent){
        Qt.exit(0)
    }
}
