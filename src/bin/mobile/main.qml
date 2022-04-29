import QtQuick 2.7
import QtQuick.Window 2.2

Window {
    id:root
    visible: true
    signal addRoll(string name, string cmd )
    signal roll(string cmd)
    color: "black"
    height: 1280
    width: 720

    Column{

        anchors.fill: parent
        leftPadding: width*0.025
        rightPadding: width*0.025
        topPadding: width*0.025
        Rectangle {
            height: parent.height*0.12
            width: parent.width*0.95
            Image {
                anchors.fill: parent
                horizontalAlignment: Image.AlignHCenter
                verticalAlignment:Image.AlignVCenter
                fillMode: Image.PreserveAspectFit
                source: "qrc:/resources/images/add.png"
            }
            gradient: Gradient {
                GradientStop { position: 0.0; color: "darkblue" }
                GradientStop { position: 1.0; color: "blue" }
            }
            MouseArea {
                anchors.fill: parent
                onClicked:popupAdd.visible = true
            }
        }

        ListView {
            model: _model
            focus: true
            height: parent.height*0.88
            width: parent.width*0.95
            delegate: Item{
                height: parent.height
                width: parent.width
                Column{
                    width: parent.width
                    height: parent.height
                    Text{
                        text: name
                        font.pointSize: 40
                        color: "white"


                    }
                    Text{
                        text: cmd
                        font.pointSize:30
                        color: "white"

                    }
                }
                MouseArea{
                    anchors.fill:parent
                    onClicked: roll(cmd)
                }
            }
        }

    }
    Rectangle {
        id: popupAdd
        color: "black"
        x: parent.width*0.1
        y: parent.height*0.1
        height: parent.height*0.80
        width: parent.width*0.80
        visible: false
        border.color: "white"
        border.width: 1

        Column{
            id:form
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: parent.height*0.80
            leftPadding: width*0.025
            rightPadding: width*0.025
            topPadding: width*0.025
            property alias name : nameInp
            property alias cmd : cmdInp
            Text {
                text: qsTr("Name")
                color: "white"
                height: parent.height*0.1
                font.pointSize: 40
            }
            Rectangle{
                height: parent.height*0.1
                width: parent.width*0.90
                border.color: "#BBBBBB"
                border.width: 1
                color: "black"
                TextInput{
                    id: nameInp
                    color: "white"
                    anchors.fill: parent
                    font.pointSize: 40
                }
            }

            Text {
                text: qsTr("Command")
                color: "white"
                height: parent.height*0.1
                font.pointSize: 40
            }
            Rectangle{
                height: parent.height*0.1
                width: parent.width*0.90
                border.color: "#BBBBBB"
                border.width: 1
                color: "black"
                TextInput{
                    id: cmdInp
                    color: "white"
                    anchors.fill: parent
                    font.pointSize: 40
                }
            }
        }

        Rectangle {
            color:"red"
            id: cancel
            anchors.top: form.bottom
            anchors.left: form.left
            anchors.right: form.horizontalCenter
            anchors.bottom: parent.bottom
            radius: height/2
            MouseArea {
                anchors.fill: parent
                onClicked:{
                    nameInp.text=""
                    cmdInp.text=""
                    popupAdd.visible = false
                }
            }
        }
        Rectangle {
            color:"green"
            anchors.top: form.bottom
            anchors.left: cancel.right
            anchors.right: form.right
            anchors.bottom: parent.bottom
            radius: height/2
            MouseArea {
                anchors.fill: parent
                onClicked:
                {
                    root.addRoll(nameInp,cmdInp.text);
                    nameInp.text=""
                    cmdInp.text=""
                    popupAdd.visible = false
                }
            }

        }
    }
}
