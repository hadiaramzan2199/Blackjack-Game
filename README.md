# Blackjack Game

This repository contains the C++ implementation of a simple console-based Blackjack game implementing OOPS (Object Oriented Programming).

## Introduction

This C++ program implements a console-based version of the classic Blackjack card game. The game involves a player and a dealer, with the goal of reaching a hand value of 21 without exceeding it.

The code defines classes for `Card`, `Hand`, `Comp` (a base class for both Player and Dealer), `Player`, `Dealer`, `Deck`, and `Game` to simulate the game. The game allows the player to hit (take another card) or stand (keep the current hand) and plays against a dealer who follows a specific set of rules.

## Usage

To play the game, compile the provided C++ code and run the executable. Follow the on-screen instructions to input your name and play the game.

## Code Structure

The code is organized as follows:

`Card class`: Represents a playing card with a rank, suit, and face-up/face-down status.

`Hand class`: Represents a player's or dealer's hand, storing a collection of cards.

`Comp class`: Abstract base class for both Player and Dealer, providing common functionality.

`Player class`: Represents a player, inherits from Comp class.

`Dealer class`: Represents a dealer, inherits from Comp class.

`Deck class`: Represents a deck of cards, manages card shuffling and dealing.

`Game class`: Represents the game of Blackjack, handles gameplay and interactions.

`main function`: Initializes the game, handles input, and executes the game loop.

## How to Play

1. Upon starting the game, you will be prompted to enter your name.
2. Follow the on-screen instructions to play the game, making decisions to hit or stay.
3. The game will compare your hand against the dealer's hand and determine the winner.
4. Enjoy playing and aim to beat the dealer without going over 21!
