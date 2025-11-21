import re
import pandas as pd

# === Config ===
INPUT_CSV  = "resume.csv"
OUTPUT_CSV = "cleaned_resume.csv"

# Set this to the column that contains the raw resume text.
# If you leave it as None, the script will auto-pick the first text-like column.
COLUMN_WITH_TEXT = "resume"  # e.g., "Summary" or "Skills" or "Description"


# --- Helpers ---

def first_sentence(text: str) -> str:
    """Take text before the first full stop (.)"""
    # If no '.', keep the whole string
    idx = text.find('.')
    return text if idx == -1 else text[:idx]

def remove_prefix_phrase(text: str) -> str:
    """
    Remove the phrase 'Experienced professional skilled in' (case-insensitive)
    from the start of the string.
    """
    return re.sub(r'^\s*Experienced professional skilled in\s*', '', text, flags=re.IGNORECASE)

def split_skills(text: str) -> list:
    """
    Split into items by common separators. Keep it simple and conservative.
    """
    # Normalize separators to commas
    text = re.sub(r'[;•|/]+', ',', text)
    # Also handle ' and ' that often appears in lists
    text = re.sub(r'\s+\band\b\s+', ',', text, flags=re.IGNORECASE)
    # Split by comma
    parts = [p.strip(" \t\n\r\"'") for p in text.split(',')]
    return [p for p in parts if p]

def starts_with_capital(item: str) -> bool:
    """
    Keep only sub-entries where the first alphabetic character is capital.
    (Step 4)
    """
    m = re.search(r'[A-Za-z]', item)
    if not m:
        return False
    return item[m.start()].isupper()

def remove_specific_words(item: str) ->  bool:
    if not isinstance(item, str) or not item.strip():
        return False
    
    banned_words = ["Democrat", "Republican"]

    lowered = item.lower()
    for w in banned_words:
        if w.lower() in lowered:
            return False
    return True

def less_than_three_not_fully_capitalized(item: str) -> bool:
    if(len(item) < 2):
        return False
    elif(len(item) < 3 and not item.isupper()):
        return False
    else:
        return True


def clean_resume_skills(raw: str) -> str:
    if not isinstance(raw, str):
        return ""

    # Step 2: take text before first full stop
    s = first_sentence(raw)

    # Step 3: remove the specific prefix phrase
    s = remove_prefix_phrase(s)

    # Tokenize into potential skills
    items = split_skills(s)

    # Step 4: remove sub-entries where text starts with a non-capital letter
    items = [it for it in items if starts_with_capital(it)]

    # Step 5: remove sub-entries where text <3 length AND not fully captitalized (e.g. 'Mr', 'Ms')
    items = [it for it in items if less_than_three_not_fully_capitalized(it)]

    # Step 6: removes specific words (e.g. "I")
    items = [it for it in items if remove_specific_words(it)]

    # Deduplicate while preserving order
    seen = set()
    cleaned = []
    for it in items:
        key = it.strip()
        if key and key not in seen:
            seen.add(key)
            cleaned.append(key)

    # Return comma separated string list
    return ", ".join(cleaned)


# --- Load & Auto-pick text column if needed ---

df = pd.read_csv(INPUT_CSV)

if COLUMN_WITH_TEXT is None:
    # Try to choose a reasonable text column automatically
    text_cols = df.select_dtypes(include=["object"]).columns.tolist()
    # Prefer common names if present
    preferred = [c for c in text_cols if c.lower() in ("summary", "skills", "description", "resume", "text")]
    if preferred:
        COLUMN_WITH_TEXT = preferred[0]
    elif text_cols:
        COLUMN_WITH_TEXT = text_cols[0]
    else:
        raise ValueError("No text-like (object dtype) columns found. Set COLUMN_WITH_TEXT to your skills/summary column.")

if COLUMN_WITH_TEXT not in df.columns:
    raise ValueError(f"Configured COLUMN_WITH_TEXT='{COLUMN_WITH_TEXT}' not found in CSV columns: {list(df.columns)}")


# --- Apply cleaning ---

# Step 1: place ResumeID (unique ID per row, sequential)
resume_ids = [f"R{str(i).zfill(6)}" for i in range(1, len(df) + 1)]

cleaned_skills = df[COLUMN_WITH_TEXT].apply(clean_resume_skills)

out = pd.DataFrame({
    "ResumeID": resume_ids,
    "Cleaned_Skills": cleaned_skills
})

# removing entries with no skills at all
out = out[out["Cleaned_Skills"].str.strip() != ""]

# Save with only the requested headers (order preserved)
out.to_csv(OUTPUT_CSV, index=False)

print(f"\nDone. Wrote {len(out)} rows to {OUTPUT_CSV}")
