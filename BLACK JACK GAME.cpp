#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

class Card
{
	public:
	enum Rank{ACE=1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING};
	enum Suit{CLUBS, DIAMONDS, HEARTS, SPADES};
	
	//overloading operator<< so we can have a standard output
	friend ostream& operator<<(ostream &out, const Card &cards);
	
	Card(Rank r=ACE, Suit s=SPADES, bool up=true);
	
	//returns the value of card from 1-11
	int Get_vals() const;
	
	//flips a card i-e face doen to face up & face up to face down
	void Flip();
	
	private:
		Rank x_rank;
		Suit x_suit;
		bool x_FaceUp;
};

//Constructor
Card::Card(Rank r, Suit s, bool up)
{
	x_rank=r;
	x_suit=s;
	x_FaceUp=up;
}

int Card::Get_vals() const
{
	//if a card is face down then its value is zero
	int value=0;
	if(x_FaceUp)
	{
		//value of a face card is equal to the number shown on it
		value=x_rank;
		
		//value is 10 for JACK, QUEEN, KING
		if(value>10)
		{
			value=10;
		}
	}
	return value;
}

void Card::Flip()
{
	x_FaceUp=!(x_FaceUp); 
}

class Hand
{
	public:
		//adds a card to the hand
		void add(Card* pCard);
		
		//clears hand of all cards
		void Clear();
		
		//gets total value of hand calculating ACE as 1 or 11 according to situation
		int Get_total() const;
		
	protected:
		vector<Card*>x_card;
};

void Hand::add(Card* pCard)
{
	x_card.push_back(pCard);
}

void Hand::Clear()
{
	//iterate through vector, freeing up all the memory
	vector<Card*>::iterator iter=x_card.begin(); //this function allows to iterate from beginning
	
	for(iter=x_card.begin(); iter!=x_card.end(); ++iter) //pre-increment in iter will allow the memory to be cleaned rght from the 1st value
	{
		delete *iter;
		*iter=0; //null pointer to avoid any kind of dangling
	}
	
	//clear the vector of pointer
	x_card.clear();
}

int Hand::Get_total() const
{
	//if no cards in hand then return zero
	if(x_card.empty())
	{
		return 0;
	}
	
	//if a 1st card has value zero, it means that its face is down then return zero
	if(x_card[0]->Get_vals()==0)
	{
		return 0;
	}
	
	//add up card values and treat ACE=1
	int total=0;
	vector<Card*>::const_iterator iter;
	
	for(iter=x_card.begin(); iter!=x_card.end(); ++iter)
	{
		total+=(*iter)->Get_vals();
	}
	
	//determine whether hand contains an ACE or not
	bool containAce=false;
	
	for(iter=x_card.begin(); iter!=x_card.end(); ++iter)
	{
		if((*iter)->Get_vals()==Card::ACE)
		{
			containAce=true;
		}
	}
	
	//if hand contain ACE and the total is very less then treat ACE=11
	if(containAce && total<=11)
	{
		//add 10 because we have already added 1 
		total+=10;
	}
	return total;
}

//an abstract class which will be used as a base class for class Player & class Dealer
class Comp : public Hand
{
	//overloading operator<< so we can have a standard output
	friend ostream& operator<<(ostream &out, const Comp &comps);
	
	public:
		Comp(const string &name="");
		virtual ~Comp();
		
		//determines whether Comp wants to continue hitting or not
		virtual bool hitting() const=0; //pure virtual function
		
		//returns whether Comp is busted or not i-e total exceeding 21
		bool busted() const;
		
		//display that Comp busts
		void bust() const;
		
	protected:
	     string x_name;	
};

//constructor
Comp::Comp(const string &name)
{
	x_name=name;
}

//destructor
Comp::~Comp() {}

bool Comp::busted() const
{
	return(Get_total()>21);
}

void Comp::bust() const
{
	cout<<"Ooopss! "<<x_name<<" has BUSTED!!!"<<endl;
}

class Player : public Comp
{
	public:
		Player(const string &name="");
		virtual ~Player();
		
		//returns whether Player wants a hit a not
		virtual bool hitting() const;
		
		//displays that Player wins
		void win() const;
		
		//displays that Player loses
		void lose() const;
		
		//displays that Player pushes (draw)
		void push() const;
};

//constructor
Player::Player(const string &name) : Comp(name) {}

//destructor
Player::~Player() {}

bool Player::hitting() const
{
	char action;
	cout<<endl;
	cout<<x_name<<", do you want a HIT or STAY? (H/S): ";
	cin>>action;
	cout<<endl;
	return(action=='h' || action=='H');
}

void Player::win() const
{
	cout<<"Yahooo! "<<x_name<<" WINS!!!"<<endl;
}

void Player::lose() const
{
	cout<<"Ooopss! "<<x_name<<" LOSES!!!"<<endl;
}

void Player::push() const
{
	cout<<"Ohhh! "<<x_name<<" PUSHES!!!"<<endl;
}

class Dealer : public Comp
{
	public:
		Dealer(const string &name="Dealer");
		virtual ~Dealer();
		
		//determines whether Dealer is hitting or not, plus Dealer always has to hit until 17, so it will always hit on 10 or less
		virtual bool hitting() const;
		
		//at start only 1 card of Dealer is visible, so it will flip over that card
		void FlipCard();
};

//constructor
Dealer::Dealer(const string &name) : Comp(name) {}

//destructor
Dealer::~Dealer() {}

bool Dealer::hitting() const
{
	return(Get_total()<=16);
}

void Dealer::FlipCard()
{
	if(!(x_card.empty()))
	{
		x_card[0]->Flip();
	}
	else
	{
		cout<<"NO CARD TO FLIP!!"<<endl;
	}
}

class Deck : public Hand
{
	public:
		Deck();
		virtual ~Deck();
		
		//we are gealing with a single deck only, so it creates a deck of 52 cards
		void Populate();
		
		//shuffles the cards
		void Shuffle();
		
		//deals 1 card to a Hand
		void Deal(Hand &hands);
		
		//gives additional card to a Player as long as he keeps hitting
		void add_cards(Comp &comps);
};

//constructor
Deck::Deck()
{
	x_card.reserve(52);
	Populate();
}

//destructor
Deck::~Deck() {}

void Deck::Populate()
{
	Clear();
	
	//create Deck
	for(int s=Card::CLUBS; s<=Card::SPADES; ++s)
	{
		for(int r=Card::ACE; r<=Card::KING; ++r)
		{
		    add(new Card(static_cast <Card::Rank>(r), static_cast <Card::Suit>(s)));
        }
	}
}

void Deck::Shuffle()
{
	random_shuffle(x_card.begin(), x_card.end());
}

void Deck::Deal(Hand &hands)
{
	if(!(x_card.empty()))
	{
		hands.add(x_card.back());
		x_card.pop_back();
	}
	else
	{
		cout<<"OUT OF CARDS, UNABLE TO DEAL"<<endl;
	}
} 

void Deck::add_cards(Comp &comps)
{
	cout<<endl;
	
	//continue to deal as long as Comp is not busted and wants another hit
	while(!(comps.busted()) && comps.hitting())
	{
		Deal(comps);
		cout<<comps<<endl;
		
		if(comps.busted())
		{
			comps.bust();
		}
	}
}

class Game
{
	public:
		Game(const vector<string> &name);
		~Game();
		
		//Plays BLACK JACK
		void play();
		
		//instructions of the game
		friend void instructions();
		
	private:
	    Deck x_deck;
		Dealer x_dealer;
		vector<Player>x_player;	
};

//constructor
Game::Game(const vector<string> &name)
{
	//create a vector of players from vector of names
	vector<string>::const_iterator pName;
	
	for(pName=name.begin(); pName!=name.end(); ++pName)
	{
		x_player.push_back(Player(*pName));
	}
	x_deck.Populate();
	x_deck.Shuffle();
}

//dectructor
Game::~Game() {}

void instructions()
{
	cout<<"			   GOAL: Reach 21 without going over.\n\n";
	cout<<"		1- The DEALER will be playing against you.\n";
	cout<<"		2- If the DEALER bursts (exceed from 21), then the PLAYER will win,\n"; 
	cout<<"		   if he remains unbusted.\n";
	cout<<"		3- If the DEALER doesn't bust and the score of the PLAYER is greater\n"; 
	cout<<"		   than the DEALER's score and remains unbusted, then PLAYER will win.\n";
	cout<<"		4- If PLAYER gets 21 and the DEALER doesn't, then PLAYER will have a BLACK JACK \n";
	cout<<"		   and will win.\n";
	cout<<"		5- If the PLAYER and DEALER both have same score, then it is a PUSH (draw).\n";
	cout<<"		6- ACE can be treated as 1 or 11 according to score's situation.\n";
	cout<<"		7- Only 1 PLAYER can play at a time.\n\n";
	cout<<"					*****GOOD LUCK*****			\n\n";
}

void Game::play()
{
	//deal initials 2 cards to both PLAYER & DEALER
	vector<Player>::iterator pPlayer;
	for(int i=0; i<2; ++i)
	{
		for(pPlayer=x_player.begin(); pPlayer!=x_player.end(); ++pPlayer)
		{
			x_deck.Deal(*pPlayer);
		}
		x_deck.Deal(x_dealer);
	}
	
	//hides DEALER 1st card
	x_dealer.FlipCard();
	
	//displays everyone's hand
	for(pPlayer=x_player.begin(); pPlayer!=x_player.end(); ++pPlayer)
	{
		cout<<*pPlayer<<endl;
	}
	
	cout<<x_dealer<<endl;
	
	//deals additional cards to player
	for(pPlayer=x_player.begin(); pPlayer!=x_player.end(); ++pPlayer)
	{
		x_deck.add_cards(*pPlayer);
	}
	
	//reveals DEALER's 1st card
	x_dealer.FlipCard();
	cout<<endl<<x_dealer;
	
	//deals additional cards to DEALER
	x_deck.add_cards(x_dealer);
	
	if(x_dealer.busted())
	{
		//player wins if not busted
		for(pPlayer=x_player.begin(); pPlayer!=x_player.end(); ++pPlayer)
		{
			if(!(pPlayer->busted()))
			{
				pPlayer->win();
			}
		}
	}
	else
	{
		//compare PLAYER with DEALER
		for(pPlayer=x_player.begin(); pPlayer!=x_player.end(); ++pPlayer)
		{
			if(!(pPlayer->busted()))
			{
				if(pPlayer->Get_total() > x_dealer.Get_total())
				{
					pPlayer->win();
				}
				else if(pPlayer->Get_total() < x_dealer.Get_total())
				{
					pPlayer->lose();
				}
				else
				{
					pPlayer->push();
				}
			}
		}
	}
	
	//removes cards of PLAYER & DEALER
	for(pPlayer=x_player.begin(); pPlayer!=x_player.end(); ++pPlayer)
	{
		pPlayer->Clear();
	}
	x_dealer.Clear();
}

//overloaded operator<< () function prototype
ostream& operator<<(ostream &out, const Card &cards);
ostream& operator<<(ostream &out, const Comp &comps);

int main()
{
	cout<<"		*****WELCOME TO BLACK JACK GAME, HAVE FUN PLAYING!*****		"<<endl;
	cout<<endl;
	instructions();
	
	vector<string>name;
	string Name;
	
	cout<<"ENTER YOUR NAME: ";
	cin>>Name;
	name.push_back(Name);
	cout<<endl;
	
	//game loop
	Game game(name);
	char ans='y';
	while(ans!='n' && ans!='N')
	{
		game.play();
		cout<<endl;
		cout<<"\nHAVING FUN PLAYING, DO YOU WANT TO PLAY AGAIN? (Y/N): ";
		cin>>ans;
		cout<<endl;
	}
	return 0;
}

//overload operator<< so that Card object can be sent to cout
ostream& operator<<(ostream &out, const Card &cards)
{
  const string RANKS[] = {"0", "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
  const string SUITS[] = {"CLUBS", "DIAMONDS", "HEARTS", "SPADES"};

  if (cards.x_FaceUp)
      out << RANKS[cards.x_rank]<<" of " << SUITS[cards.x_suit];
  else
      out << "XX";

  return out;
}

//overloads << operator so a Comp object can be sent to cout
ostream& operator<<(ostream &out, const Comp &comps)
{
    out<<comps.x_name<< ":\t";

    vector <Card*>::const_iterator pCard;
    if(!comps.x_card.empty())
    {
        for(pCard=comps.x_card.begin(); pCard!=comps.x_card.end(); ++pCard)
        {
            out<<*(*pCard)<<"\t";
        }
        if(comps.Get_total()!=0)
        {
            cout<<"("<<comps.Get_total()<<")";
        }
	}
    else
    {
      out <<"(empty)";
    }
  return out;
}
