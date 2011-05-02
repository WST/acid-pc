QT      += network xml multimedia

INCLUDEPATH += \
           $$PWD/qxmpp/src

#DEFINES += QXMPP_USE_SPEEX

#LIBS += -lspeex

HEADERS += $$PWD/qxmpp/src/QXmppUtils.h \
           $$PWD/qxmpp/src/QXmppArchiveIq.h \
           $$PWD/qxmpp/src/QXmppArchiveManager.h \
           $$PWD/qxmpp/src/QXmppBindIq.h \
           $$PWD/qxmpp/src/QXmppByteStreamIq.h \
           $$PWD/qxmpp/src/QXmppCallManager.h \
           $$PWD/qxmpp/src/QXmppClient.h \
           $$PWD/qxmpp/src/QXmppClientExtension.h \
           $$PWD/qxmpp/src/QXmppCodec.h \
           $$PWD/qxmpp/src/QXmppConfiguration.h \
           $$PWD/qxmpp/src/QXmppConstants.h \
           $$PWD/qxmpp/src/QXmppDataForm.h \
           $$PWD/qxmpp/src/QXmppDialback.h \
           $$PWD/qxmpp/src/QXmppDiscoveryIq.h \
           $$PWD/qxmpp/src/QXmppDiscoveryManager.h \
           $$PWD/qxmpp/src/QXmppElement.h \
           $$PWD/qxmpp/src/QXmppEntityTimeIq.h \
           $$PWD/qxmpp/src/QXmppEntityTimeManager.h \
           $$PWD/qxmpp/src/QXmppGlobal.h \
           $$PWD/qxmpp/src/QXmppIbbIq.h \
           $$PWD/qxmpp/src/QXmppIncomingClient.h \
           $$PWD/qxmpp/src/QXmppIncomingServer.h \
           $$PWD/qxmpp/src/QXmppInvokable.h \
           $$PWD/qxmpp/src/QXmppIq.h \
           $$PWD/qxmpp/src/QXmppJingleIq.h \
           $$PWD/qxmpp/src/QXmppLogger.h \
           $$PWD/qxmpp/src/QXmppMessage.h \
           $$PWD/qxmpp/src/QXmppMucIq.h \
           $$PWD/qxmpp/src/QXmppMucManager.h \
           $$PWD/qxmpp/src/QXmppNonSASLAuth.h \
           $$PWD/qxmpp/src/QXmppOutgoingClient.h \
           $$PWD/qxmpp/src/QXmppOutgoingServer.h \
           $$PWD/qxmpp/src/QXmppPacket.h \
           $$PWD/qxmpp/src/QXmppPingIq.h \
           $$PWD/qxmpp/src/QXmppPresence.h \
           $$PWD/qxmpp/src/QXmppPubSubIq.h \
           $$PWD/qxmpp/src/QXmppRoster.h \
           $$PWD/qxmpp/src/QXmppRosterIq.h \
           $$PWD/qxmpp/src/QXmppRosterManager.h \
		   $$PWD/qxmpp/src/QXmppRtpChannel.h \
           $$PWD/qxmpp/src/QXmppSaslAuth.h \
           $$PWD/qxmpp/src/QXmppSrvInfo.h \
		   $$PWD/qxmpp/src/QXmppSrvInfo_p.h \
           $$PWD/qxmpp/src/QXmppSessionIq.h \
           $$PWD/qxmpp/src/QXmppSocks.h \
           $$PWD/qxmpp/src/QXmppStanza.h \
           $$PWD/qxmpp/src/QXmppStream.h \
           $$PWD/qxmpp/src/QXmppStreamFeatures.h \
           $$PWD/qxmpp/src/QXmppStreamInitiationIq.h \
           $$PWD/qxmpp/src/QXmppStun.h \
           $$PWD/qxmpp/src/QXmppTransferManager.h \
           $$PWD/qxmpp/src/QXmppReconnectionManager.h \
           $$PWD/qxmpp/src/QXmppRemoteMethod.h \
           $$PWD/qxmpp/src/QXmppRpcIq.h \
           $$PWD/qxmpp/src/QXmppVCardManager.h \
           $$PWD/qxmpp/src/QXmppVCardIq.h \
           $$PWD/qxmpp/src/QXmppVCard.h \
           $$PWD/qxmpp/src/QXmppVersionIq.h\
           $$PWD/qxmpp/src/QXmppVersionManager.h \
			  $$PWD/qxmpp/src/QXmppPasswordChecker.h

SOURCES += $$PWD/qxmpp/src/QXmppUtils.cpp \
           $$PWD/qxmpp/src/QXmppArchiveIq.cpp \
           $$PWD/qxmpp/src/QXmppArchiveManager.cpp \
           $$PWD/qxmpp/src/QXmppBindIq.cpp \
           $$PWD/qxmpp/src/QXmppByteStreamIq.cpp \
           $$PWD/qxmpp/src/QXmppCallManager.cpp \
           $$PWD/qxmpp/src/QXmppClient.cpp \
           $$PWD/qxmpp/src/QXmppClientExtension.cpp \
           $$PWD/qxmpp/src/QXmppCodec.cpp \
           $$PWD/qxmpp/src/QXmppConfiguration.cpp \
           $$PWD/qxmpp/src/QXmppConstants.cpp \
           $$PWD/qxmpp/src/QXmppDataForm.cpp \
           $$PWD/qxmpp/src/QXmppDialback.cpp \
           $$PWD/qxmpp/src/QXmppDiscoveryIq.cpp \
           $$PWD/qxmpp/src/QXmppDiscoveryManager.cpp \
           $$PWD/qxmpp/src/QXmppElement.cpp \
           $$PWD/qxmpp/src/QXmppEntityTimeIq.cpp \
           $$PWD/qxmpp/src/QXmppEntityTimeManager.cpp \
           $$PWD/qxmpp/src/QXmppGlobal.cpp \
           $$PWD/qxmpp/src/QXmppIbbIq.cpp \
           $$PWD/qxmpp/src/QXmppIncomingClient.cpp \
           $$PWD/qxmpp/src/QXmppIncomingServer.cpp \
           $$PWD/qxmpp/src/QXmppInvokable.cpp \
           $$PWD/qxmpp/src/QXmppIq.cpp \
           $$PWD/qxmpp/src/QXmppJingleIq.cpp \
           $$PWD/qxmpp/src/QXmppLogger.cpp \
           $$PWD/qxmpp/src/QXmppMessage.cpp \
           $$PWD/qxmpp/src/QXmppMucIq.cpp \
           $$PWD/qxmpp/src/QXmppMucManager.cpp \
           $$PWD/qxmpp/src/QXmppNonSASLAuth.cpp \
           $$PWD/qxmpp/src/QXmppOutgoingClient.cpp \
           $$PWD/qxmpp/src/QXmppOutgoingServer.cpp \
           $$PWD/qxmpp/src/QXmppPacket.cpp \
           $$PWD/qxmpp/src/QXmppPingIq.cpp \
           $$PWD/qxmpp/src/QXmppPresence.cpp \
           $$PWD/qxmpp/src/QXmppPubSubIq.cpp \
           $$PWD/qxmpp/src/QXmppRosterIq.cpp \
           $$PWD/qxmpp/src/QXmppRosterManager.cpp \
		   $$PWD/qxmpp/src/QXmppRtpChannel.cpp \
           $$PWD/qxmpp/src/QXmppSaslAuth.cpp \
		   $$PWD/qxmpp/src/QXmppSrvInfo.cpp \
           $$PWD/qxmpp/src/QXmppSessionIq.cpp \
           $$PWD/qxmpp/src/QXmppSocks.cpp \
           $$PWD/qxmpp/src/QXmppStanza.cpp \
           $$PWD/qxmpp/src/QXmppStream.cpp \
           $$PWD/qxmpp/src/QXmppStreamFeatures.cpp \
           $$PWD/qxmpp/src/QXmppStreamInitiationIq.cpp \
           $$PWD/qxmpp/src/QXmppStun.cpp \
           $$PWD/qxmpp/src/QXmppTransferManager.cpp \
           $$PWD/qxmpp/src/QXmppReconnectionManager.cpp \
           $$PWD/qxmpp/src/QXmppRemoteMethod.cpp \
           $$PWD/qxmpp/src/QXmppRpcIq.cpp \
           $$PWD/qxmpp/src/QXmppVCardManager.cpp \
           $$PWD/qxmpp/src/QXmppVCardIq.cpp \
           $$PWD/qxmpp/src/QXmppVersionIq.cpp \
           $$PWD/qxmpp/src/QXmppVersionManager.cpp \
			  $$PWD/qxmpp/src/QXmppPasswordChecker.cpp
