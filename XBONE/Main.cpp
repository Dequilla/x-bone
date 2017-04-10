#include <iostream>

#include <SFML/Graphics.hpp>

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Window.hpp>
#include <SFGUI/Desktop.hpp>
#include <SFGUI/Widgets.hpp>

#include "ControllerInterface.h"

int main(int argc, char** argv)
{
	WORD leftMotorSpeed = 0;
	WORD rightMotorSpeed = 0;
	bool runVibrations = false; // used to determine wether or not to actually run
	bool activeController[4] = { true, false, false, false }; // Used to determine the controllers the user wants vibrating

	ControllerInterface controllers;
	
	/// Setup gui ///
	sfg::SFGUI sfgui;

	// Window
	auto window = sfg::Window::Create(sfg::Window::Style::BACKGROUND);
	window->SetTitle("XBONE");
	window->SetRequisition(sf::Vector2f(200, 400));

	// Desktop
	sfg::Desktop desktop;
	desktop.Add(window);

	// Box
	auto scaleBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	auto buttonBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	auto labelBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);

	auto mainBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);

	// Button
	auto button = sfg::Button::Create("START");
	
	button->SetRequisition(sf::Vector2f(150.f, 25.f));

	button->GetSignal(sfg::Widget::OnLeftClick).Connect([&button, &runVibrations] {
		if (runVibrations == false)
		{
			button->SetLabel("STOP");
			runVibrations = true;
		}
		else
		{
			button->SetLabel("START");
			runVibrations = false;
		}
	});

	// Scales
	auto scaleLeft = sfg::Scale::Create(sfg::Scale::Orientation::VERTICAL);
	auto scaleRight = sfg::Scale::Create(sfg::Scale::Orientation::VERTICAL);

	scaleLeft->SetRequisition(sf::Vector2f(50.f, 250.f));
	scaleRight->SetRequisition(sf::Vector2f(50.f, 250.f));
	
	// Labels for scales
	auto leftLabel = sfg::Label::Create("LS: 0");
	auto rightLabel = sfg::Label::Create("RS: 0");
	
	// Adjustment for scales
	auto adjustmentLeft = scaleLeft->GetAdjustment();
	auto adjustmentRight = scaleRight->GetAdjustment();

	adjustmentLeft->SetLower(0.f);
	adjustmentRight->SetLower(0.f);

	adjustmentLeft->SetUpper(65535.f);
	adjustmentRight->SetUpper(65535.f);

	adjustmentLeft->SetMinorStep(100.f);
	adjustmentRight->SetMinorStep(100.f);

	adjustmentLeft->SetMajorStep(500.f);
	adjustmentRight->SetMajorStep(500.f);

	//															 Lambda/freefunction
	adjustmentLeft->GetSignal(sfg::Adjustment::OnChange).Connect([&adjustmentLeft, &leftLabel, &leftMotorSpeed] {
		leftMotorSpeed = adjustmentLeft->GetValue();
		std::stringstream ss;
		ss << adjustmentLeft->GetValue();
		leftLabel->SetText("LS: " + ss.str());
	});

	adjustmentRight->GetSignal(sfg::Adjustment::OnChange).Connect([&adjustmentRight, &rightLabel, &rightMotorSpeed] {
		rightMotorSpeed = adjustmentRight->GetValue();
		std::stringstream ss;
		ss << adjustmentRight->GetValue();
		rightLabel->SetText("RS: " + ss.str());
	});

	// Pack it up
	scaleBox->Pack(scaleLeft);
	scaleBox->Pack(scaleRight);

	buttonBox->Pack(button);

	labelBox->Pack(leftLabel);
	labelBox->Pack(rightLabel);

	mainBox->Pack(scaleBox);
	mainBox->Pack(labelBox);
	mainBox->Pack(buttonBox);

	window->Add(mainBox);

	sf::RenderWindow renderWindow(sf::VideoMode(200, 400), "XBONE", sf::Style::Titlebar | sf::Style::Close);
	renderWindow.resetGLStates();
	/// Setup gui ///

	std::cout << std::endl;
	std::cout << "Welcome to: " << std::endl;
	std::cout << "  X    X         XXX    XXX   XX    X  XXXXX  X" << std::endl;
	std::cout << "   X  X          X  X  X   X  X X   X  X      X" << std::endl;
	std::cout << "    XX    xxxx   XXX   X   X  X  X  X  XXX    X" << std::endl;
	std::cout << "   X  X          X  X  X   X  X   X X  X       " << std::endl;
	std::cout << "  X    X         XXX    XXX   X    XX  XXXXX  X" << std::endl << "(Yes indeed I have no naming creativity)" << std::endl << std::endl;
 
	std::cout << "You have " << controllers.getConnected() << " controllers connected!" << std::endl;
	std::cout << "Take note that currently only the first connected controller is used!" << std::endl;

	sf::Event e;
	sf::Clock clock;
	int milliseconds = 0;
	while (renderWindow.isOpen())
	{
		while (renderWindow.pollEvent(e))
		{
			desktop.HandleEvent(e);

			if (e.type == sf::Event::Closed)
				renderWindow.close();
		}

		sf::Time deltaTime = clock.restart();

		desktop.Update(deltaTime.asSeconds());

		milliseconds += deltaTime.asMilliseconds();
		if (runVibrations && milliseconds > 300)
		{
			controllers.vibrate(0, leftMotorSpeed, rightMotorSpeed);

			milliseconds = 0;
		}

		renderWindow.clear();
		sfgui.Display(renderWindow);
		renderWindow.display();
	}


	return 0;
}