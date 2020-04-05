bool collision(const sf::ConvexShape& obj1, const sf::ConvexShape& obj2)
{
    sf::Vector2f axes[obj1.getPointCount() + obj2.getPointCount()];
    sf::Vector2f obj1_points[obj1.getPointCount()];
    sf::Vector2f obj2_points[obj2.getPointCount()];

    for(int i = 0; i < obj1.getPointCount(); ++i)
        obj1_points[i] = obj1.getTransform().transformPoint(obj1.getPoint(i));
    for(int i = 0; i < obj2.getPointCount(); ++i)
        obj2_points[i] = obj2.getTransform().transformPoint(obj2.getPoint(i));

    for(int i = 0; i < obj1.getPointCount(); ++i)
        axes[i] = obj1_points[i] - obj1_points[(i+1)%obj1.getPointCount()];
    for(int i = 0; i < obj2.getPointCount(); ++i)
        axes[i + obj1.getPointCount()] = obj2_points[i] - obj2_points[(i+1)%obj2.getPointCount()];

    bool collision = true;
    for(auto axis : axes)
    {
        axis /= std::sqrt(axis.x * axis.x + axis.y * axis.y);
        std::swap(axis.x, axis.y);
        axis.y *= -1;
        float obj1_min, obj1_max, obj2_min, obj2_max;

        obj1_min = obj2_min = std::numeric_limits<float>::max();
        obj1_max = obj2_max = -std::numeric_limits<float>::max();

        for(int i = 0; i < obj1.getPointCount(); ++i)
        {
            float projection = obj1_points[i].x * axis.x + obj1_points[i].y * axis.y;
            obj1_min = std::min(obj1_min, projection);
            obj1_max = std::max(obj1_max, projection);
        }
        for(int i = 0; i < obj2.getPointCount(); ++i)
        {
            float projection = obj2_points[i].x * axis.x + obj2_points[i].y * axis.y;
            obj2_min = std::min(obj2_min, projection);
            obj2_max = std::max(obj2_max, projection);
        }


        if(obj1_min > obj2_max || obj2_min > obj1_max)
        {
            collision = false;
        }
        if(!collision)
            break;
    }
    return collision;
}