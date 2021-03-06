//------------------------------------------------------------------------------
/*
	Copyright (c) 2012, 2013 Skywell Labs Inc.
	Copyright (c) 2017-2018 TrueChain Foundation.

    Permission to use, copy, modify, and/or distribute this software for any
    purpose  with  or without fee is hereby granted, provided that the above
    copyright notice and this permission notice appear in all copies.

    THE  SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
    WITH  REGARD  TO  THIS  SOFTWARE  INCLUDING  ALL  IMPLIED  WARRANTIES  OF
    MERCHANTABILITY  AND  FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
    ANY  SPECIAL ,  DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
    WHATSOEVER  RESULTING  FROM  LOSS  OF USE, DATA OR PROFITS, WHETHER IN AN
    ACTION  OF  CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
//==============================================================================

#include <BeastConfig.h>
#include <protocol/Indexes.h>

namespace truechain {

// get the index of the node that holds the last 256 ledgers
uint256
getLedgerHashIndex ()
{
    Serializer s (2);
    s.add16 (spaceSkipList);
    return s.getSHA512Half ();
}

// Get the index of the node that holds the set of 256 ledgers that includes
// this ledger's hash (or the first ledger after it if it's not a multiple
// of 256).
uint256
getLedgerHashIndex (std::uint32_t desiredLedgerIndex)
{
    Serializer s (6);
    s.add16 (spaceSkipList);
    s.add32 (desiredLedgerIndex >> 16);
    return s.getSHA512Half ();
}

// get the index of the node that holds the enabled amendments
uint256
getLedgerAmendmentIndex ()
{
    Serializer s (2);
    s.add16 (spaceAmendment);
    return s.getSHA512Half ();
}

// get the index of the node that holds the fee schedule
uint256
getLedgerFeeIndex ()
{
    Serializer s (2);
    s.add16 (spaceFee);
    return s.getSHA512Half ();
}

uint256
getAccountRootIndex (Account const& account)
{
    Serializer  s (22);

    s.add16 (spaceAccount);
    s.add160 (account);

    return s.getSHA512Half ();
}

uint256
getAccountRootIndex (const SkywellAddress & account)
{
    return getAccountRootIndex (account.getAccountID ());
}

uint256
getGeneratorIndex (Account const& uGeneratorID)
{
    Serializer  s (22);

    s.add16 (spaceGenerator);
    s.add160 (uGeneratorID);

    return s.getSHA512Half ();
}

uint256
getBookBase (Book const& book)
{
    Serializer  s (82);

    assert (isConsistent (book));

    s.add16 (spaceBookDir);
    s.add160 (book.in.currency);
    s.add160 (book.out.currency);
    s.add160 (book.in.account);
    s.add160 (book.out.account);

    // Return with quality 0.
    return getQualityIndex (s.getSHA512Half ());
}

uint256
getOfferIndex (Account const& account, std::uint32_t uSequence)
{
    Serializer  s (26);

    s.add16 (spaceOffer);
    s.add160 (account);
    s.add32 (uSequence);

    return s.getSHA512Half ();
}

uint256
getOwnerDirIndex (Account const& account)
{
    Serializer  s (22);

    s.add16 (spaceOwnerDir);
    s.add160 (account);

    return s.getSHA512Half ();
}


uint256
getDirNodeIndex (uint256 const& uDirRoot, const std::uint64_t uNodeIndex)
{
    if (uNodeIndex)
    {
        Serializer  s (42);

        s.add16 (spaceDirNode);
        s.add256 (uDirRoot);
        s.add64 (uNodeIndex);

        return s.getSHA512Half ();
    }
    else
    {
        return uDirRoot;
    }
}

uint256
getQualityIndex (uint256 const& uBase, const std::uint64_t uNodeDir)
{
    // Indexes are stored in big endian format: they print as hex as stored.
    // Most significant bytes are first.  Least significant bytes represent
    // adjacent entries.  We place uNodeDir in the 8 right most bytes to be
    // adjacent.  Want uNodeDir in big endian format so ++ goes to the next
    // entry for indexes.
    uint256 uNode (uBase);

    // TODO(tom): there must be a better way.
    ((std::uint64_t*) uNode.end ())[-1] = htobe64 (uNodeDir);

    return uNode;
}

uint256
getQualityNext (uint256 const& uBase)
{
    static uint256 uNext ("10000000000000000");
    return uBase + uNext;
}

std::uint64_t
getQuality (uint256 const& uBase)
{
    return be64toh (((std::uint64_t*) uBase.end ())[-1]);
}

uint256
getTicketIndex (Account const& account, std::uint32_t uSequence)
{
    Serializer  s (26);

    s.add16 (spaceTicket);
    s.add160 (account);
    s.add32 (uSequence);

    return s.getSHA512Half ();
}

uint256
getSkywellStateIndex (Account const& a, Account const& b, Currency const& currency)
{
    Serializer  s (62);

    s.add16 (spaceSkywell);

    if (a < b)
    {
        s.add160 (a);
        s.add160 (b);
    }
    else
    {
        s.add160 (b);
        s.add160 (a);
    }

    s.add160 (currency);

    return s.getSHA512Half ();
}

uint256
getSkywellStateIndex (Account const& a, Issue const& issue)
{
    return getSkywellStateIndex (a, issue.account, issue.currency);
}


uint256 getTrustStateIndex (Account const& a, Account const& b, std::uint32_t uType,Account const& issuer,Currency const& currency)
{
    Serializer  s (62);

    s.add16 (spaceTrust);

    if (a < b)
    {
        s.add160 (a);
        s.add160 (b);
    }
    else
    {
        s.add160 (b);
        s.add160 (a);
    }
    s.add32 (uType);

    s.add160 (issuer);
    s.add160 (currency);

    return s.getSHA512Half ();
}

uint256 getSignStateIndex(Account const& a)
{
	Serializer  s(22);

	s.add16(spaceSign);

	s.add160(a);

	return s.getSHA512Half();
}

uint256 getNickNameIndex(Account  const &a)
{

	Serializer  s(22);

	s.add16(spaceNickname);

	s.add160(a);

	return s.getSHA512Half();
}
uint256
getNickNameIndex(const SkywellAddress & account)
{
	return  getNickNameIndex(account.getAccountID());
}
}
