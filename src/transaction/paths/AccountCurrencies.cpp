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
#include <transaction/paths/AccountCurrencies.h>

namespace truechain {

CurrencySet accountSourceCurrencies (
    SkywellAddress const& raAccountID,
    SkywellLineCache::ref lrCache,
    bool includeSWT)
{
    CurrencySet currencies;

    // YYY Only bother if they are above reserve
    if (includeSWT)
        currencies.insert (xrpCurrency());

    // List of truechain lines.
    auto& truechainLines (lrCache->getSkywellLines (raAccountID.getAccountID ()));

    for (auto const& item : truechainLines)
    {
        auto rspEntry = (SkywellState*) item.get ();
        assert (rspEntry);
        if (!rspEntry)
            continue;

        auto& saBalance = rspEntry->getBalance ();

        // Filter out non
        if (saBalance > zero
            // Have IOUs to send.
            || (rspEntry->getLimitPeer ()
                // Peer extends credit.
                && ((-saBalance) < rspEntry->getLimitPeer ()))) // Credit left.
        {
            currencies.insert (saBalance.getCurrency ());
        }
    }

    currencies.erase (badCurrency());
    return currencies;
}

CurrencySet accountDestCurrencies (
    SkywellAddress const& raAccountID,
    SkywellLineCache::ref lrCache,
    bool includeSWT)
{
    CurrencySet currencies;

    if (includeSWT)
        currencies.insert (xrpCurrency());
    // Even if account doesn't exist

    // List of truechain lines.
    auto& truechainLines (lrCache->getSkywellLines (raAccountID.getAccountID ()));

    for (auto const& item : truechainLines)
    {
        auto rspEntry = (SkywellState*) item.get ();
        assert (rspEntry);
        if (!rspEntry)
            continue;

        auto& saBalance  = rspEntry->getBalance ();

        if (saBalance < rspEntry->getLimit ())                  // Can take more
            currencies.insert (saBalance.getCurrency ());
    }

    currencies.erase (badCurrency());
    return currencies;
}

} // truechain
