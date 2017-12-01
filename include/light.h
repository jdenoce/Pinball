
struct Light
{
    GLenum m_LightID;
    glm::vec4 m_Ambient;
    glm::vec4 m_Diffuse;
    glm::vec4 m_Specular;
 
    glm::vec4 m_Position;
    glm::vec3 m_SpotDirection;
    float  m_SpotExponent;
    float  m_SpotCutoff;
    float  m_ConstantAttenuation;
    float  m_LinearAttenuation;
    float  m_QuadraticAttenuation;

    Light( GLenum lightID = GL_LIGHT0
         , glm::vec4 ambient = glm::vec4( 0.0, 0.0, 0.0, 1.0 )
         , glm::vec4 diffuse = glm::vec4( 1.0, 1.0, 1.0, 1.0 )
         , glm::vec4 specular = glm::vec4( 1.0, 1.0, 1.0, 1.0 )
         , glm::vec4 position = glm::vec4( 0.0, 0.0, 1.0, 0.0 )
         , glm::vec3 spotDirection = glm::vec3( 0.0, 0.0, 1.0 )
         , float  spotExponent = 0.0
         , float  spotCutoff = 180.0f
         , float  constantAttenuation = 1.0
         , float  linearAttenuation = 0.0
         , float  quadraticAttenuation = 0.0 )
        : m_LightID ( lightID )
        , m_Ambient( ambient )
        , m_Diffuse( diffuse )
        , m_Specular( specular )
        , m_Position( position )
        , m_SpotDirection( spotDirection )
        , m_SpotExponent( spotExponent )
        , m_SpotCutoff( spotCutoff )
        , m_ConstantAttenuation( constantAttenuation )
        , m_LinearAttenuation( linearAttenuation )
        , m_QuadraticAttenuation( quadraticAttenuation )
    {}
 
    void Activate()
    {
        glEnable( m_LightID );
        glLightfv( m_LightID, GL_AMBIENT, &(m_Ambient.r) );
        glLightfv( m_LightID, GL_DIFFUSE, &(m_Diffuse.r) );
        glLightfv( m_LightID, GL_SPECULAR, &(m_Specular.r) );
        glLightfv( m_LightID, GL_POSITION, &(m_Position.x) );
        glLightfv( m_LightID, GL_SPOT_DIRECTION, &(m_SpotDirection.x) );
        glLightf( m_LightID, GL_SPOT_EXPONENT, m_SpotExponent );
        glLightf( m_LightID, GL_SPOT_CUTOFF, m_SpotCutoff );
        glLightf( m_LightID, GL_CONSTANT_ATTENUATION, m_ConstantAttenuation );
        glLightf( m_LightID, GL_LINEAR_ATTENUATION, m_LinearAttenuation );
        glLightf( m_LightID, GL_QUADRATIC_ATTENUATION, m_QuadraticAttenuation );
    } 
 
    void Deactivate()
    {
        glDisable( m_LightID );
    }
 

};
