#pragma once

#include "eos_platform_prereqs.h"
#include "eos_common.h"
#include "eos_presence_types.h"

#pragma pack(push, 8)


/** The most recent version of the EOS_PresenceModification_SetTemplateId API. */
#define EOS_PRESENCEMODIFICATION_SETTEMPLATEID_API_LATEST 1

/**
 * Data for the EOS_PresenceModification_SetTemplateId function.
 */
EOS_STRUCT(EOS_PresenceModification_SetTemplateIdOptions, (
	/** API Version: Set this to EOS_PRESENCEMODIFICATION_SETTEMPLATEID_API_LATEST. */
	int32_t ApiVersion;
	/** The RichPresence Template ID. Setting this value will prevent SetRawRichText from being used on this handle */
	const char* TemplateId;
));

/** Most recent version of the EOS_PresenceModification_SetTemplateData API */
#define EOS_PRESENCEMODIFICATION_SETTEMPLATEDATA_API_LATEST 1

/**
 * Enum representing the types that may be passed as template data. 
 */
EOS_ENUM(EOS_EPresenceModificationTemplateType,
	/** (32-bit) integer type */
	EOS_PMT_INT = 1,
	/** UTF8 String as an identifier */
	EOS_PMT_STRING = 2
);

/**
 *  Data for the EOS_PresenceModification_SetTemplateData API.
 */
EOS_STRUCT(EOS_PresenceModification_SetTemplateDataOptions, (
	/** API Version: Set this to EOS_PRESENCEMODIFICATION_SETTEMPLATEDATA_API_LATEST. */
	int32_t ApiVersion;
	/** Key for the named template parameter */
	const char* Key;
	/** Union storage for template typed values */
	union
	{
		/** Localized integer */
		int32_t AsInt32;
		/** Reference a StringId in the Backend */
		const char* AsStringId;
	} Value;
	/** Type stored in the union */
	EOS_EPresenceModificationTemplateType ValueType;
));

#pragma pack(pop)
