using System;
using System.Collections.Generic;
using System.Text;
using UnityEngine;

public enum Language
{
    Spanish = 0,
    English = 1,
    French = 2,
    German = 3
}

/*****************************************************************/
//Esta clase gestiona la base de datos de textos multi-idioma a partir
//de un CSV, usando un patrón Singleton accesible desde cualquier
//escena.
//Carga y parsea el fichero (ID;SPANISH;ENGLISH;...), almacenando los
//textos por ID e idioma, con idioma por defecto y fallbacks.
//Expone una API (GetText, GetInfo, GetNombre, SGet…) y un evento para
//notificar cambios de idioma al resto de la UI y sistemas.
/*****************************************************************/


public class PlanetTextCSVLoader : MonoBehaviour
{
    // ---------- Singleton ----------
    public static PlanetTextCSVLoader Instance { get; private set; }
    [Tooltip("Si está activo, este loader persiste entre escenas.")]
    public bool dontDestroyOnLoad = true;

    // ---------- Config (TEXTOS SIMPLES) ----------
    [Header("Texto genérico (multi-idioma)")]
    public TextAsset csvFile;                           
    public string resourcesPath = "planet_texts_multi"; 

    // =======================

    [Header("Idioma por defecto")]
    public Language currentLanguage;
    private Language defaultLanguage = Language.Spanish;

    // ---------- Datos TEXTO SIMPLE ----------
    // id -> (Language -> text)
    public Dictionary<string, Dictionary<Language, string>> db
        = new Dictionary<string, Dictionary<Language, string>>(StringComparer.OrdinalIgnoreCase);

    // Idiomas detectados en cabecera (en orden)
    public List<Language> languages = new List<Language>();

    // =======================

    public event Action languageChange;

    void Awake()
    {
        if (Instance != null && Instance != this) { Destroy(gameObject); return; }
        Instance = this;
        if (dontDestroyOnLoad) DontDestroyOnLoad(gameObject);

        Load();         // textos simples

        currentLanguage = defaultLanguage;
    }

    // ---------- API TEXTO SIMPLE ----------
    public static string SGet(string id) => Instance ? Instance.GetText(id) : id;
    public static string SGet(string id, Language lang) => Instance ? Instance.GetText(id, lang) : id;

    [Obsolete("Usa SGet(id, Language)")]
    public static string SGet(string id, string lang) => Instance ? Instance.GetText(id, lang) : id;

    public string GetText(string id) => GetText(id, currentLanguage);

    public string GetInfo(PlanetClickable planet, GameMode mode)
    {

        int mode_int = 0;

        switch (mode)
        {
            case GameMode.Kid:
                mode_int = 0;
                break;
            case GameMode.Normal:
                mode_int = 1;
                break;
            case GameMode.Expert:
                mode_int = 2;
                break;
        }

        
        string info_id = planet.GetId() + "_info_" + mode_int;
        return GetText(info_id);
    }

    public string GetNombre(PlanetClickable p)
    {
        string nombre_id = p.GetId() + "_nombre";
        return GetText(nombre_id);
    }

    public string GetText(string id, Language language)
    {
        if (string.IsNullOrEmpty(id)) return "";
        if (!db.TryGetValue(id, out var perLang) || perLang == null || perLang.Count == 0) return id;

        // 1) idioma pedido
        if (perLang.TryGetValue(language, out var t) && !string.IsNullOrEmpty(t)) return t;

        // 2) idioma por defecto
        if (perLang.TryGetValue(defaultLanguage, out var td) && !string.IsNullOrEmpty(td)) return td;

        // 3) primer idioma disponible según cabecera
        foreach (var lang in languages)
            if (perLang.TryGetValue(lang, out var any) && !string.IsNullOrEmpty(any)) return any;

        return id;
    }

    public string GetText(string id, string language)
    {
        if (TryParseLanguage(language, out var lang))
            return GetText(id, lang);
        return GetText(id); 
    }

    public void setLanguage(Language new_language)
    {
        if (currentLanguage != new_language)
        {
            currentLanguage = new_language;
            //Debug.Log("IDIOMA CAMBIADO");
            languageChange?.Invoke();
        }
    }

    [ContextMenu("Reload CSV")]
    public void Load()
    {
        string csv = null;

        if (csvFile != null) csv = csvFile.text;
        else
        {
            var ta = Resources.Load<TextAsset>(resourcesPath);
            if (ta != null) csv = ta.text;
        }

        if (string.IsNullOrEmpty(csv))
        {
            Debug.LogError("[PlanetTextCSVLoader] No se encontró CSV de textos simples.");
            db.Clear(); languages.Clear();
            return;
        }

        Parse(csv); 
#if UNITY_EDITOR
        Debug.Log($"[PlanetTextCSVLoader] TEXTOS OK. IDs={db.Count}. Idiomas=[{string.Join(", ", languages)}]. Default={defaultLanguage}");
#endif
    }

    // ---------- Parser TEXTO SIMPLE (delimitador ';' fijo, soporta comillas) ----------
    private void Parse(string csv)
    {
        db.Clear();
        languages.Clear();

        int i = 0;
        var header = ReadLine(csv, ref i); // delim ';'
        if (header.Count < 2)
        {
            Debug.LogError("[PlanetTextCSVLoader] Cabecera inválida. Esperado: ID;SPANISH;ENGLISH;...");
            return;
        }

        if (!header[0].Equals("ID", StringComparison.OrdinalIgnoreCase))
            Debug.LogWarning($"[PlanetTextCSVLoader] Primera columna no es 'ID' sino '{header[0]}'. Se usará igualmente.");

        var colToLang = new Dictionary<int, Language>();
        for (int c = 1; c < header.Count; c++)
        {
            var raw = header[c]?.Trim();
            if (TryParseLanguage(raw, out var lang))
            {
                colToLang[c] = lang;
                if (!languages.Contains(lang)) languages.Add(lang);
            }
            else
            {
                Debug.LogWarning($"[PlanetTextCSVLoader] Columna de idioma desconocida '{raw}' ignorada.");
            }
        }

        while (i < csv.Length)
        {
            var row = ReadLine(csv, ref i);
            if (row.Count == 0) continue;

            string id = row.Count > 0 ? row[0]?.Trim() : null;
            if (string.IsNullOrEmpty(id)) continue;

            var perLang = new Dictionary<Language, string>();
            foreach (var kv in colToLang)
            {
                int c = kv.Key;
                var lang = kv.Value;
                string val = (c < row.Count) ? row[c] : "";
                perLang[lang] = val;
            }
            db[id] = perLang;
        }
    }



    private static List<string> ReadLine(string s, ref int i)
    {
        const char delimiter = ';';
        var fields = new List<string>();
        var sb = new StringBuilder();
        bool inQuotes = false;

        while (i < s.Length)
        {
            char ch = s[i++];

            if (inQuotes)
            {
                if (ch == '"')
                {
                    if (i < s.Length && s[i] == '"') { sb.Append('"'); i++; } // "" -> "
                    else inQuotes = false;
                }
                else sb.Append(ch);
            }
            else
            {
                if (ch == delimiter) { fields.Add(sb.ToString()); sb.Clear(); }
                else if (ch == '\r') { /* ignore */ }
                else if (ch == '\n') break;
                else if (ch == '"') inQuotes = true;
                else sb.Append(ch);
            }
        }
        fields.Add(sb.ToString());
        return fields;
    }

    // --------- Helpers idioma ---------
    public static bool TryParseLanguage(string s, out Language lang)
    {
        lang = default;
        if (string.IsNullOrWhiteSpace(s)) return false;

        s = s.Trim();

        switch (s.ToUpperInvariant())
        {
            case "ES":
            case "ES-ES":
            case "SPANISH":
            case "CASTILIAN":
                lang = Language.Spanish; return true;
            case "EN":
            case "EN-GB":
            case "EN-US":
            case "ENGLISH":
                lang = Language.English; return true;
            case "FR":
            case "FR-FR":
            case "FRENCH":
                lang = Language.French; return true;
            case "DE":
            case "DE-DE":
            case "GERMAN":
                lang = Language.German; return true;
        }

        return Enum.TryParse(s, true, out lang);
    }
}
