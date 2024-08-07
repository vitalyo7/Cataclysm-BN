#include "catch/catch.hpp"

#include <string>

#include "calendar.h"
#include "item.h"
#include "type_id.h"

// Test cases focused on item::type_name

TEST_CASE( "item name pluralization", "[item][type_name][plural]" )
{
    SECTION( "singular and plural item names" ) {

        SECTION( "plural is the same as singular" ) {
            item &lead = *item::spawn_temporary( "lead" );
            item &gold = *item::spawn_temporary( "gold_small" );

            CHECK( lead.type_name( 1 ) == "lead" );
            CHECK( lead.type_name( 2 ) == "lead" );

            CHECK( gold.type_name( 1 ) == "gold" );
            CHECK( gold.type_name( 2 ) == "gold" );
        }

        SECTION( "pluralize the last part" ) {
            item &rag = *item::spawn_temporary( "rag" );
            item &mattress = *item::spawn_temporary( "mattress" );
            item &incendiary = *item::spawn_temporary( "incendiary" );
            item &plastic = *item::spawn_temporary( "plastic_chunk" );

            // just add +s
            CHECK( rag.type_name( 1 ) == "rag" );
            CHECK( rag.type_name( 2 ) == "rags" );
            CHECK( plastic.type_name( 1 ) == "plastic chunk" );
            CHECK( plastic.type_name( 2 ) == "plastic chunks" );

            // -y to +ies
            CHECK( incendiary.type_name( 1 ) == "incendiary" );
            CHECK( incendiary.type_name( 2 ) == "incendiaries" );

            // -ss to +sses
            CHECK( mattress.type_name( 1 ) == "mattress" );
            CHECK( mattress.type_name( 2 ) == "mattresses" );

        }

        SECTION( "pluralize the first part" ) {
            item &glass = *item::spawn_temporary( "glass_sheet" );
            item &cards = *item::spawn_temporary( "deck_of_cards" );
            item &jar = *item::spawn_temporary( "jar_eggs_pickled" );

            CHECK( glass.type_name( 1 ) == "sheet of glass" );
            CHECK( glass.type_name( 2 ) == "sheets of glass" );

            CHECK( cards.type_name( 1 ) == "deck of cards" );
            CHECK( cards.type_name( 2 ) == "decks of cards" );

            CHECK( jar.type_name( 1 ) == "sealed jar of eggs" );
            CHECK( jar.type_name( 2 ) == "sealed jars of eggs" );
        }

        SECTION( "pluralize by inserting a word" ) {
            item &mag = *item::spawn_temporary( "mag_archery" );
            item &book = *item::spawn_temporary( "SICP" );

            CHECK( mag.type_name( 1 ) == "Archery for Kids" );
            CHECK( mag.type_name( 2 ) == "issues of Archery for Kids" );

            CHECK( book.type_name( 1 ) == "SICP" );
            CHECK( book.type_name( 2 ) == "copies of SICP" );
        }
    }
}

TEST_CASE( "custom named item", "[item][type_name][named]" )
{
    // Shop smart. Shop S-Mart.
    item &shotgun = *item::spawn_temporary( "shotgun_410" );
    shotgun.set_var( "name", "Boomstick" );
    REQUIRE( shotgun.get_var( "name" ) == "Boomstick" );

    CHECK( shotgun.type_name() == "Boomstick" );
}

TEST_CASE( "corpse item", "[item][type_name][corpse]" )
{
    static const mtype_id mon_zombie( "mon_zombie" );
    static const mtype_id mon_chicken( "mon_chicken" );

    // Anonymous corpses

    SECTION( "human corpse" ) {
        item &corpse = *item::make_corpse( mtype_id::NULL_ID(), calendar::turn, "" );
        REQUIRE( corpse.is_corpse() );
        REQUIRE( corpse.get_corpse_name().empty() );

        CHECK( corpse.type_name() == "corpse of a human" );
    }

    SECTION( "zombie corpse" ) {
        item &corpse = *item::make_corpse( mon_zombie, calendar::turn, "" );
        REQUIRE( corpse.is_corpse() );
        REQUIRE( corpse.get_corpse_name().empty() );

        CHECK( corpse.type_name() == "corpse of a zombie" );
    }

    SECTION( "chicken corpse" ) {
        item &corpse = *item::make_corpse( mon_chicken, calendar::turn, "" );
        REQUIRE( corpse.is_corpse() );
        REQUIRE( corpse.get_corpse_name().empty() );

        CHECK( corpse.type_name() == "corpse of a chicken" );
    }

    // Corpses with names

    SECTION( "human corpse with a name" ) {
        item &corpse = *item::make_corpse( mtype_id::NULL_ID(), calendar::turn, "Dead Dude" );
        REQUIRE( corpse.is_corpse() );
        REQUIRE_FALSE( corpse.get_corpse_name().empty() );

        CHECK( corpse.type_name() == "corpse of Dead Dude, human" );
    }

    SECTION( "zombie corpse with a name" ) {
        item &corpse = *item::make_corpse( mon_zombie, calendar::turn, "Deadite Jones" );
        REQUIRE( corpse.is_corpse() );
        REQUIRE_FALSE( corpse.get_corpse_name().empty() );

        CHECK( corpse.type_name() == "corpse of Deadite Jones, zombie" );
    }

    SECTION( "chicken corpse with a name" ) {
        item &corpse = *item::make_corpse( mon_chicken, calendar::turn, "Herb" );
        REQUIRE( corpse.is_corpse() );
        REQUIRE_FALSE( corpse.get_corpse_name().empty() );

        CHECK( corpse.type_name() == "corpse of Herb, chicken" );
    }
}

