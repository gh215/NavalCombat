#include "pch.h"
#include "CppUnitTest.h"
#include "..\NavalCombat2\naval.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(UnitTests)
	{
	public:
		TEST_METHOD(TestShip_hascell)
		{
			Ship ship({ {'A', 1}, {'B', 1}, {'C', 1} });
			Assert::IsTrue(ship.has_cell({ 'A', 1 }));
			Assert::IsFalse(ship.has_cell({ 'D', 1 }));
		}
		TEST_METHOD(TestShip_cellstate)
		{
			Ship ship({ {'A', 1}, {'B', 1}, {'C', 1} });
			Assert::AreEqual(ship.cell_state({ 'A', 1 }), PIPE);

			ship.hit({ 'A', 1 });
			Assert::AreEqual(ship.cell_state({ 'A', 1 }), WOUNDED);
		}
		TEST_METHOD(TestShip_hit)
		{
			Ship ship({ {'A', 1} });
			ship.hit({ 'A', 1 });

			Assert::AreEqual(ship.cell_state({ 'A', 1 }), KILLED);
		}
		TEST_METHOD(TestShip_killedcheck)
		{

			Ship ship({ {'A', 1}, {'B', 1} });
			ship.hit({ 'A', 1 });
			ship.hit({ 'B', 1 });

			Assert::IsTrue(ship.killed);
		}
		TEST_METHOD(TestShip_near)
		{
			Ship ship({ {'A', 1}, {'B', 1} });
			cell nearCell = { 'A', 2 };
			cell farCell = { 'D', 1 };

			Assert::IsTrue(ship.near(nearCell));
			Assert::IsFalse(ship.near(farCell));
		}
		TEST_METHOD(TestBoard_all_ships_killed)
		{
			Board board;
			Ship ship1({ {'A', 1}, {'A', 2} });
			Ship ship2({ {'B', 1}, {'B', 2} });
			board.put_ship(ship1);
			board.put_ship(ship2);

			board.shoot({ 'A', 1 });
			board.shoot({ 'A', 2 });
			board.shoot({ 'B', 1 });
			board.shoot({ 'B', 2 });

			Assert::IsTrue(board.all_ships_killed());
		}
		TEST_METHOD(TestBoard_celltarget)
		{
			Board board;
			Ship ship({ {'A', 1}, {'A', 2} });
			board.put_ship(ship);

			Assert::AreEqual(board.shoot({ 'A', 1 }), WOUNDED);
			Assert::AreEqual(board.shoot({ 'B', 1 }), MISSED);
		}
		TEST_METHOD(TestBoard_cellstate)
		{
			Board board;
			cell emptyCell = { 'A', 1 };
			cell missedCell = { 'B', 1 };

			Assert::AreEqual(board.cell_state(emptyCell), EMPTY);

			board.shoot(missedCell);
			Assert::AreEqual(board.cell_state(missedCell), MISSED);
		}
		TEST_METHOD(TestBoard_putship)
		{
			Board board;
			vector<cell> shipCoords = { {'A', 1}, {'A', 2} };
			Ship ship(shipCoords);

			Assert::IsTrue(board.put_ship(ship));
		}
		TEST_METHOD(TestBoard_ship_nearby)
		{
			Board board;
			vector<cell> ship1Coords = { {'A', 1}, {'A', 2} };
			Ship ship1(ship1Coords);
			board.put_ship(ship1);

			//Ship_nearby = True
			Assert::IsTrue(board.ship_nearby({ 'A', 3 }));
			//Ship_nearby = False
			Assert::IsFalse(board.ship_nearby({ 'C', 3 }));
		}
		TEST_METHOD(TestBoard_Shoot)
		{
			Board board;
			vector<cell> shipCoords = { {'A', 1}, {'A', 2} };
			Ship ship(shipCoords);
			board.put_ship(ship);

			//Result = Wounded
			char result = board.shoot({ 'A', 1 });

			Assert::AreEqual(WOUNDED, result);

			//Result = Missed
			char result1 = board.shoot({ 'B', 1 });

			Assert::AreEqual(MISSED, result1);

			//Result = Killed
			board.shoot({ 'A', 1 });
			char result2 = board.shoot({ 'A', 2 });

			Assert::AreEqual(KILLED, result2);
		}
	};
}
