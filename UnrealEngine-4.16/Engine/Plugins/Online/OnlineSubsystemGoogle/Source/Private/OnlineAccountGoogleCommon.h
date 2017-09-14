// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once
 
#include "OnlineSubsystemGoogleTypes.h"
#include "OnlineJsonSerializer.h"
#include "OnlineSubsystemGooglePackage.h"
#include "IHttpRequest.h"

/**
 * Info associated with an user account generated by this online service
 */
class FUserOnlineAccountGoogleCommon : 
	public FUserOnlineAccount,
	public FOnlineJsonSerializable
{
public:

	// FOnlineUser
	
	virtual TSharedRef<const FUniqueNetId> GetUserId() const override;
	virtual FString GetRealName() const override;
	virtual FString GetDisplayName(const FString& Platform = FString()) const override;
	virtual bool GetUserAttribute(const FString& AttrName, FString& OutAttrValue) const override;
	virtual bool SetUserAttribute(const FString& AttrName, const FString& AttrValue) override;

	// FUserOnlineAccount

	virtual FString GetAccessToken() const override;
	virtual bool GetAuthAttribute(const FString& AttrName, FString& OutAttrValue) const override;

	// FUserOnlineAccountGoogleCommon

	explicit FUserOnlineAccountGoogleCommon(const FString& InUserId = FString(), const FAuthTokenGoogle& InAuthToken = FAuthTokenGoogle())
		: UserIdPtr(new FUniqueNetIdString(InUserId))
		, UserId(InUserId)
		, AuthToken(InAuthToken)
	{
	}

	virtual ~FUserOnlineAccountGoogleCommon()
	{
	}

	/**
	 * Parse Json profile request data into the user account
	 *
	 * @param InAuthToken previously associated auth token with this Json response
	 * @param InJsonStr a json payload from a /me request
	 */
	bool Parse(const FAuthTokenGoogle& InAuthToken, const FString& InJsonStr);

protected:

	/**
	 * Get account data attribute
	 *
	 * @param Key account data entry key
	 * @param OutVal [out] value that was found
	 *
	 * @return true if entry was found
	 */
	inline bool GetAccountData(const FString& Key, FString& OutVal) const
	{
		const FString* FoundVal = AccountData.Find(Key);
		if (FoundVal != NULL)
		{
			OutVal = *FoundVal;
			return true;
		}
		return false;
	}

	/**
	 * Set account data attribute
	 *
	 * @param Key account data entry key
	 * @param Val value that we want to set
	 *
	 * @return true if entry changed
	 */
	inline bool SetAccountData(const FString& Key, const FString& Val)
	{
		const FString* FoundVal = AccountData.Find(Key);
		if (FoundVal == NULL || *FoundVal != Val)
		{
			AccountData.Add(Key, Val);
			return true;
		}
		return false;
	}

	void AddUserAttributes(const TSharedPtr<FJsonObject>& JsonUser);

	/** Any addition account data associated with the user */
	FJsonSerializableKeyValueMap AccountData;

	/** User Id represented as a FUniqueNetId */
	TSharedRef<const FUniqueNetId> UserIdPtr;
	/** Id associated with the user account provided by the online service during registration */
	FString UserId;
	/** Real name */
	FString RealName;
	/** First name */
	FString FirstName;
	/** Last name */
	FString LastName;
	/** Token which is provided to user once authenticated by the online service */
	FAuthTokenGoogle AuthToken;

private:

	// FJsonSerializable
	BEGIN_ONLINE_JSON_SERIALIZER
		ONLINE_JSON_SERIALIZE("id", UserId); //v2
		ONLINE_JSON_SERIALIZE("sub", UserId); //v3
		ONLINE_JSON_SERIALIZE("given_name", FirstName);
		ONLINE_JSON_SERIALIZE("family_name", LastName);
		ONLINE_JSON_SERIALIZE("name", RealName);
	END_ONLINE_JSON_SERIALIZER

	/** Allow the Google identity to fill in our private members from it's callbacks */
	friend class FOnlineIdentityGoogle;
};
