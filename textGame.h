#pragma once

#ifndef TEXTGAME
#define TEXTGAME

#include <QtWidgets>
#include <QObject>
#include <QApplication>
#include <QSocketNotifier>
#include "game.h"


// Console section, to be separated into another file
class TextGame: public QObject {
    Q_OBJECT;
	Game* game;

	public:
		TextGame(Game*);
		void run();

	signals:
		void quit();

	private:
		std::unique_ptr<QSocketNotifier> m_notifier;

	private slots:
		void readCommand();
};
#endif