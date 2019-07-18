#pragma once

#ifndef TEXTGAME
#define TEXTGAME

#include <iostream>

#include <QtWidgets>
#include <QObject>
#include <QApplication>
#include <QSocketNotifier>
#include "game.h"


/*
The text game object is responsible for managing the central game state and
communication between outside world and the game.
*/
class TextGame: public QObject {
    Q_OBJECT;

	public:
		/*
		 Initialize the text display and socket notifier for cin, we detect
		 user input using the file descriptor 
		 */
		TextGame(Game*);
		/*
		 Connect the file descriptor change signal with readCommand(), so it
		 knows when to perform readCommand();
		 */
		void run();

	signals:
		/*
	 	The signal sent to parent to quit QApp
		 */
		void quit();
		/*
		The signal sent to parent and will connect to the GUI's update slot
		 */
        void updateWindow();

	public slots:
		/*
		Execute arbitrary string commands, so we can accomodate various input
		devices and methods
		 */
		void execCommand(std::string command);

	private:
		Game* game;
		std::unique_ptr<QSocketNotifier> m_notifier;

	private slots:
		void readCommand(std::istream& = std::cin);
};
#endif