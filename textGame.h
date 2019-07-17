#pragma once

#ifndef TEXTGAME
#define TEXTGAME

#include <iostream>

#include <QtWidgets>
#include <QObject>
#include <QApplication>
#include <QSocketNotifier>
#include "game.h"


// Console section, to be separated into another file
class TextGame: public QObject {
    Q_OBJECT;

	public:
		TextGame(Game*);
		void run();

	signals:
		void quit();
        void updateWindow();

	public slots:
		void execCommand(std::string command);

	private:
		Game* game;
		std::unique_ptr<QSocketNotifier> m_notifier;

	private slots:
		void readCommand(std::istream& = std::cin);
};
#endif