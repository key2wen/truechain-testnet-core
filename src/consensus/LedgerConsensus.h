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

#ifndef SKYWELL_APP_CONSENSUS_LEDGERCONSENSUS_H_INCLUDED
#define SKYWELL_APP_CONSENSUS_LEDGERCONSENSUS_H_INCLUDED

#include <chrono>
#include <ledger/Ledger.h>
#include <ledger/LedgerProposal.h>
#include <common/misc/CanonicalTXSet.h>
#include <common/misc/FeeVote.h>
#include <common/json/json_value.h>
#include <transaction/tx/LocalTxs.h>
#include <network/overlay/Peer.h>
#include <protocol/SkywellLedgerHash.h>

namespace truechain {

/** Manager for achieving consensus on the next ledger.

    This object is created when the consensus process starts, and
    is destroyed when the process is complete.
*/
class LedgerConsensus
{
public:
    virtual ~LedgerConsensus() = 0;

    virtual int startup () = 0;

    virtual Json::Value getJson (bool full) = 0;

    virtual Ledger::ref peekPreviousLedger () = 0;

    virtual uint256 getLCL () = 0;

    virtual void mapComplete (uint256 const& hash,
        std::shared_ptr<SHAMap> const& map, bool acquired) = 0;

    virtual void checkLCL () = 0;

    virtual void handleLCL (uint256 const& lclHash) = 0;

    virtual void timerEntry () = 0;

    // state handlers
    virtual void statePreClose () = 0;
    virtual void stateEstablish () = 0;
    virtual void stateFinished () = 0;
    virtual void stateAccepted () = 0;

    virtual bool haveConsensus (bool forReal) = 0;

    virtual bool peerPosition (LedgerProposal::ref) = 0;

    virtual bool isOurPubKey (const SkywellAddress & k) = 0;

    // test/debug
    virtual void simulate () = 0;
};

std::shared_ptr <LedgerConsensus>
make_LedgerConsensus (LocalTxs& localtx,
    LedgerHash const & prevLCLHash, Ledger::ref previousLedger,
        std::uint32_t closeTime, FeeVote& feeVote);

void
applyTransactions(std::shared_ptr<SHAMap> const& set, Ledger::ref applyLedger,
                  Ledger::ref checkLedger,
                  CanonicalTXSet& retriableTransactions, bool openLgr);

} // truechain

#endif
